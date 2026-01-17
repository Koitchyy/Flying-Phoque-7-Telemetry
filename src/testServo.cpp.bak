#include <Arduino.h>
#include <PinDefinitions.h>
#include <Servo.h>
#include <peripherals/Bilda.h>

Bilda airbrakes;

Servo bilda2000;
int airbrake_direction = 1;
unsigned long last_airbrake_update = 0;
float airbrake_pct = 0.0;

void setup() {
  Serial1.begin(115200);
  Serial1.println("Starting servo test");
  // bilda2000.attach(PA1);
  // Serial1.println("Moving servo to middle position");
  // bilda2000.writeMicroseconds(1500); // middle position
  airbrakes.begin(PinDefs.SERVO);
}

int MIN_EXTENSION = 500;
// int MAX_EXTENSION = 2000;
int MAX_EXTENSION = 1700;
int STAGES = 5;
int delayTime = 750;

void loop() {
  // for (int i = 1; i <= STAGES; i++) {
  //   int us = MIN_EXTENSION + (MAX_EXTENSION - MIN_EXTENSION) * i / STAGES;
  //   Serial1.print("Moving servo to ");
  //   Serial1.print(us);
  //   Serial1.println(" us");
  //   bilda2000.writeMicroseconds(us);
  //   delay(delayTime);
  // }
  // delay(1000);
  // bilda2000.writeMicroseconds(MIN_EXTENSION);
  // delay(1000);

  // Sweep: 0% to 60% and back, 10% steps, 1.0s hold

  if (millis() - last_airbrake_update >= 1000) {
    last_airbrake_update = millis();

    Serial1.print("Setting extension to: ");
    Serial1.print(airbrake_pct);
    Serial1.println("%");
    airbrakes.setExtension(airbrake_pct);

    airbrake_pct += 10.0 * airbrake_direction;

    // Clamp and reverse direction
    if (airbrake_pct >= 60.0) {
      airbrake_pct = 60.0;
      airbrake_direction = -1;
    } else if (airbrake_pct <= 0.0) {
      airbrake_pct = 0.0;
      airbrake_direction = 1;
    }
  }
}
