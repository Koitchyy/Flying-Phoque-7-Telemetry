#include <Arduino.h>
#include <Servo.h>

Servo bilda2000;

void setup() {
  Serial1.begin(115200);
  Serial1.println("Starting servo test");
  bilda2000.attach(PA1);
  Serial1.println("Moving servo to middle position");
  bilda2000.writeMicroseconds(1500); // middle position
}

void loop() {
  Serial1.println("Moving servo to 500 us");
  bilda2000.writeMicroseconds(500);
  delay(500);

  Serial1.println("Moving servo to 750 us");
  bilda2000.writeMicroseconds(750);
  delay(500);

  Serial1.println("Moving servo to 1000 us");
  bilda2000.writeMicroseconds(1000);
  delay(500);

  Serial1.println("Moving servo to 1250 us");
  bilda2000.writeMicroseconds(1250);
  delay(500);

  Serial1.println("Moving servo to 1500 us");
  bilda2000.writeMicroseconds(1500);
  delay(500);

  Serial1.println("Moving servo to 1750 us");
  bilda2000.writeMicroseconds(1750);
  delay(500);

  Serial1.println("Moving servo to 2000 us");
  bilda2000.writeMicroseconds(2000);
  delay(500);

  Serial1.println("Moving servo to 2250 us");
  bilda2000.writeMicroseconds(2250);
  delay(500);

  Serial1.println("Moving servo to 2500 us");
  bilda2000.writeMicroseconds(2500);
  delay(500);
}
