/**********************************************************************
 *  Swap-in version that uses SparkFun_BNO080_Arduino_Library instead
 *  of Adafruit_BNO08x.  All other code is unchanged.
 *********************************************************************/
#define TEENSY_I2C_ADDR 0x42
#define I2C_BUFFER_LENGTH 128

#include <Arduino.h>
#include <Logging.h>
#include <PinDefinitions.h>
#include <SD.h>
#include <SPI.h>
#include <States.h>
#include <Wire.h>
#include <peripherals/Bilda.h>
#include <peripherals/Igniter.h>
#include <peripherals/StatusIndicator.h>
#include <sensor_drivers/Adxl.h>
#include <sensor_drivers/BNO.h>
#include <sensor_drivers/Lps22.h>
#include <telemetry/telStruct.h>

const bool SIMULATION_MODE = false;
const bool USE_BNO080 = true;

// Telemetry related //
const bool USE_TELEMETRY = false;
unsigned long lastTelemetrySend = 0;

float accel_x, accel_y, accel_z;
float accel_x_high_g, accel_y_high_g, accel_z_high_g;

uint32_t mode;
float position, velocity, current, torque;

float pressure;
float temperature;

float p_ref = 0, t_ref = 0;

int fire_time = 0;
float max_altitude = 0.0;

float bno_x, bno_y, bno_z;           // linear accel
float bno_i, bno_j, bno_k, bno_real; // quaternion (rotation vector)

bool primaryIgniterConnected = false;
bool backupIgniterConnected = false;

// Airbrake control globals
int ignition_time = 0;
int motor_burnout_time = 0;
int last_actuation_time = 0;
float airbrake_pct = 0.0;
int airbrake_direction = 1; // 1 = extend, -1 = retract
unsigned long last_airbrake_update = 0;

Logging logging(true, true, PinDefs.SD_CS);
File dataFile;
Adxl adxl345 = Adxl(0x1D, ADXL345);
Adxl adxl375 = Adxl(0x53, ADXL375);
Lps22 lps22 = Lps22(0x5C);
Igniter primaryIgniter = Igniter(PinDefs.IGNITER_0, PinDefs.IGNITER_SENSE_0);
Igniter backupIgniter = Igniter(PinDefs.IGNITER_1, PinDefs.IGNITER_SENSE_1);
StatusIndicator statusIndicator = StatusIndicator(
    PinDefs.STATUS_LED_RED, PinDefs.STATUS_LED_GREEN, PinDefs.STATUS_LED_BLUE);
BNO bno080;
Bilda airbrakes;

// State machine //
States state = States::BOOT;

int last_sd_write = 0;

/* ---------- HELPER ------------------------------------------------- */
void splitString(String data, char delimiter, String parts[],
                 int maxParts) { /* unchanged */ }

/* ---------- SETUP -------------------------------------------------- */
void setup() {
  Serial1.begin(115200);

  pinMode(PinDefs.ARM, INPUT_PULLUP);
  pinMode(PinDefs.IGNITER_0, OUTPUT);
  pinMode(PinDefs.IGNITER_1, OUTPUT);

  airbrakes.begin(PinDefs.SERVO);

  while (!Serial1) {
    statusIndicator.solid(StatusIndicator::RED);
  }
  statusIndicator.solid(StatusIndicator::RED);

  // Hardware I2C initialization
  Wire.setSDA(PinDefs.SDA);
  Wire.setSCL(PinDefs.SCL);
  Wire.begin();
  Wire.setClock(100000); // 100 kHz

  delay(4000); // allow sensors to power up

  // SPI init
  SPI.setMOSI(PinDefs.SDI);
  SPI.setMISO(PinDefs.SDO);
  SPI.setSCLK(PinDefs.SCK);
  SPI.begin();

  // ---------------- SD / Logging ----------------
  while (!logging.begin()) {
    statusIndicator.solid(StatusIndicator::RED);
    Serial1.println(F("Waiting for SD card..."));
    delay(1000);
  }

  Serial1.println(F("SD card initialized"));

  // ---------------- Sensors ---------------------
  // ADXL345
  int adxl345_attempts = 0;
  while (!adxl345.begin()) {
    logging.log("Err ADXL345");
    Serial1.println("Waiting for ADXL345...");
    delay(1000);
    adxl345_attempts++;
    if (adxl345_attempts >= 10) {
      break;
    }
  }
  Serial1.println("ADXL345 initialized");

  // ADXL375
  int adxl375_attempts = 0;
  while (!adxl375.begin()) {
    logging.log("Err ADXL375");
    Serial1.println("Waiting for ADXL375...");
    delay(1000);
    adxl375_attempts++;
    if (adxl375_attempts >= 10) {
      break;
    }
  }
  Serial1.println("ADXL375 initialized");

  // LPS22
  int lps_attempts = 0;
  while (!lps22.begin()) {
    logging.log("Err LPS22");
    Serial1.println("Waiting for LPS22...");
    delay(1000);
    lps_attempts++;
    if (lps_attempts >= 10) {
      break;
    }
  }
  Serial1.println("LPS22 initialized");

  // BNO080 (optional)
  if (USE_BNO080) {
    Serial1.println("Attempting BNO080 initialization...");
    int bno_attempts = 0;
    unsigned long bno_start_time = millis();
    const unsigned long bno_timeout = 15000;

    while (bno_attempts < 100) {
      if (millis() - bno_start_time > bno_timeout) {
        Serial1.println("BNO080 initialization timeout. Giving up.");
        break;
      }

      if (bno080.begin()) {
        Serial1.println("BNO080 initialized successfully!");
        break;
      }

      logging.log("ErrBNO080");
      Serial1.print("Waiting for BNO080 (attempt ");
      Serial1.print(bno_attempts + 1);
      Serial1.println("/10)...");
      delay(1000);
      bno_attempts++;
    }

    if (bno_attempts >= 100) {
      Serial1.println("BNO080 initialization failed. Continuing without BNO.");
    }
  }

  // ---------------- Reference readings ----------------
  for (int i = 0; i < 10; i++) {
    float pressure, temperature;
    lps22.readPressure(&pressure);
    lps22.readTemperature(&temperature);
    p_ref += pressure;
  }
  p_ref /= 10.0;

  // ---------------- Log header ----------------
  const String logHeading =
      "Time\tXg\tYg\tZg\tXhg\tYhg\tZhg\tPressure\tTemperature\tAltitude\t"
      "BNO_X\tBNO_Y\tBNO_Z\tBNO_I\tBNO_J\tBNO_K\tBNO_Real\tState\tAirbrake_"
      "pct\tAirbrake_dir";
  logging.log(logHeading.c_str());
  logging.flush();

  statusIndicator.solid(StatusIndicator::GREEN);
  Serial1.println("Setup complete");
  state = States::IDLE;
}

float altitudeDelta(float p_ref, float p, float T_ref, float T) {
  if (SIMULATION_MODE) {
    p_ref = 1013.25;
    T_ref = 293.15;
  }
  const float Rd = 287.05f;                 // J/(kg·K)
  const float g0 = 9.80665f;                // m/s^2
  float Tbar = 0.5f * (T_ref + T);          // mean temperature (K)
  return (Rd * Tbar / g0) * log(p_ref / p); // meters
}

/* ---------- LOOP --------------------------------------------------- */
void loop() {
  /* Local sensors */
  if (SIMULATION_MODE) {
    Serial1.println("DATAREQUEST");
    String line = Serial1.readStringUntil('\n');
    line.trim(); // remove newline or spaces

    int lastIndex = 0;
    int index = 0;
    float values[6];

    for (int i = 0; i < line.length(); i++) {
      if (line[i] == ',' || i == line.length() - 1) {
        String part =
            line.substring(lastIndex, (i == line.length() - 1) ? i + 1 : i);
        values[index++] = part.toFloat();
        lastIndex = i + 1;
      }
    }

    // Assign them directly
    if (index >= 6) {
      accel_x = values[1];
      accel_y = values[2];
      accel_z = values[3];
      pressure = values[4];
      temperature = values[5];
    }
  } else {
    adxl345.readAccelerometer(&accel_x, &accel_y, &accel_z);
    adxl375.readAccelerometer(&accel_x_high_g, &accel_y_high_g,
                              &accel_z_high_g);
    lps22.readPressure(&pressure);
    lps22.readTemperature(&temperature);
  }

  float altitude = altitudeDelta(p_ref, pressure, t_ref, temperature + 273.15);

  /* ------------  B N O 0 8 0  DATA  ------------------------------ */
  if (USE_BNO080) {
    if (bno080.dataAvailable()) {
      bno080.getLinearAccelerometer(&bno_x, &bno_y, &bno_z);
      bno080.getRotationVector(&bno_i, &bno_j, &bno_k, &bno_real);
    }
  }

  const String logMessage =
      String(millis()) + ", " + String(accel_x) + ", " + // ADXL345
      String(accel_y) + ", " + String(accel_z) + ", " + String(accel_x_high_g) +
      ", " + // ADXL375 (high-g)
      String(accel_y_high_g) + ", " + String(accel_z_high_g) + ", " +
      String(pressure) + ", " + String(temperature) + ", " + String(altitude) +
      ", " + String(bno_x) + ", " + String(bno_y) + ", " + String(bno_z) +
      ", " + String(bno_i) + ", " + String(bno_j) + ", " + String(bno_k) +
      ", " + String(bno_real) + ", " + stateToString(state) + ", " +
      String(airbrake_pct) + ", " + String(airbrake_direction);

  logging.log(logMessage.c_str());

  if (millis() - last_sd_write > 1000) {
    logging.flush();
    last_sd_write = millis();
  }

  // State machine
  switch (state) {
  case States::IDLE:
    statusIndicator.solid(StatusIndicator::GREEN);

    if (accel_y > 8.0) {
      state = States::IGNITION;
      ignition_time = millis();
    }
    break;
  case States::IGNITION:
    statusIndicator.solid(StatusIndicator::ORANGE);

    if (accel_y < 0) {
      motor_burnout_time = millis();
      state = States::ASCENT;
    }

    break;
  case States::ASCENT:
    statusIndicator.solid(StatusIndicator::RED);

    max_altitude = max(max_altitude, altitude);

    // Airbrake control logic
    if (millis() - ignition_time > 12000 ||
        millis() - motor_burnout_time > 8000) {

      // Sweep: 0% to 60% and back, 10% steps, 1.0s hold (1.5s when fully
      // closed)
      if (millis() - last_airbrake_update >=
          (airbrake_pct <= 0 ? 1500 : 1000)) {
        last_airbrake_update = millis();

        airbrake_pct += 30.0 * airbrake_direction;

        // Clamp and reverse direction
        if (airbrake_pct >= 60.0) {
          airbrake_pct = 60.0;
          airbrake_direction = -2; // bring straight back to 0
        } else if (airbrake_pct <= 0.0) {
          airbrake_pct = 0.0;
          airbrake_direction = 1;
        }
        airbrakes.setExtension(airbrake_pct);
      }
    }

    // Check for apogee conditions (time based)
    if (millis() - ignition_time > 30000) {
      airbrakes.setExtension(0.0);
      airbrake_pct = 0.0;
      airbrake_direction = 1;
      state = States::APOGEE;
      fire_time = millis();
    }

    break;
  case States::APOGEE:
    statusIndicator.solid(StatusIndicator::RED);

    primaryIgniter.fire();

    if (millis() - fire_time > 2000) {
      state = States::DESCENT;
    }

    break;
  case States::DESCENT:
    statusIndicator.solid(StatusIndicator::BLUE);

    if (altitude < 5.0) {
      state = States::LANDED;
    }

    break;
  case States::LANDED:
    statusIndicator.solid(StatusIndicator::GREEN);

    break;
  default:
    state = States::IDLE;
    break;
  }

  delay(50);

  // if (USE_TELEMETRY &&
  //     (millis() - lastTelemetrySend >= 50U)) { // send every 50 ms
  //   TelemetryPacket pkt;

  //   pkt.time_ms = millis();
  //   pkt.accel_x = accel_x;
  //   pkt.accel_y = accel_y;
  //   pkt.accel_z = accel_z;
  //   pkt.accel_x_high_g = accel_x_high_g;
  //   pkt.accel_y_high_g = accel_y_high_g;
  //   pkt.accel_z_high_g = accel_z_high_g;
  //   pkt.pressure = pressure;
  //   pkt.temperature = temperature;
  //   pkt.altitude = altitude;
  //   pkt.bno_x = bno_x;
  //   pkt.bno_y = bno_y;
  //   pkt.bno_z = bno_z;
  //   pkt.bno_i = bno_i;
  //   pkt.bno_j = bno_j;
  //   pkt.bno_k = bno_k;
  //   pkt.bno_real = bno_real;
  //   pkt.state = static_cast<uint8_t>(state);
  //   pkt.crc = computeCRC((uint8_t *)&pkt, sizeof(pkt) - 1);

  //   Wire.beginTransmission(TEENSY_I2C_ADDR);
  //   Wire.write((uint8_t *)&pkt, sizeof(pkt));
  //   uint8_t result = Wire.endTransmission();

  //   if (result != 0) {
  //     delay(5);
  //     Wire.beginTransmission(TEENSY_I2C_ADDR);
  //     Wire.write((uint8_t *)&pkt, sizeof(pkt));
  //     result = Wire.endTransmission();
  //     if (result != 0) {
  //       Serial1.print("I2C retry failed: ");
  //       Serial1.println(result);
  //     }
  //   }
  //   Serial1.print("Telemetry packet size: ");
  //   Serial1.println(sizeof(TelemetryPacket));

  //   lastTelemetrySend = millis();
  // }
}
