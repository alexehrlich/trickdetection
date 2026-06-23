#include "Arduino_BMI270_BMM150.h"

#define WAIT_TIME 500     // How often to run the code (in milliseconds)

float acc_x, acc_y, acc_z;
float mag_x, mag_y, mag_z;
float gyr_x, gyr_y, gyr_z;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  if (IMU.accelerationAvailable() &&
      IMU.gyroscopeAvailable()) {

    IMU.readAcceleration(acc_x, acc_y, acc_z);
    IMU.readGyroscope(gyr_x, gyr_y, gyr_z);
    //IMU.readMagneticField(mag_x, mag_y, mag_z);

    // CSV Zeile ausgeben
    Serial.print(millis()); Serial.print(",");
    Serial.print(acc_x);    Serial.print(",");
    Serial.print(acc_y);    Serial.print(",");
    Serial.print(acc_z);    Serial.print(",");
    Serial.print(gyr_x);    Serial.print(",");
    Serial.print(gyr_y);    Serial.print(",");
    Serial.println(gyr_z);    //Serial.print(",");
    // Serial.print(mag_x);    Serial.print(",");
    // Serial.print(mag_y);    Serial.print(",");
    // Serial.println(mag_z);
  }
}