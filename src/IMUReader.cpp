#include "IMUReader.hpp"

IMUReader::IMUReader() : _ready(false) {}

bool IMUReader::begin() {
    if (!IMU.begin()) {
        Serial.println("IMU init failed");
        _ready = false;
        return false;
    }
    Serial.print("Accelerometer sample rate: ");
    Serial.println(IMU.accelerationSampleRate());
    Serial.print("Gyroscope sample rate: ");
    Serial.println(IMU.gyroscopeSampleRate());
    _ready = true;
    return true;
}

bool IMUReader::isAvailable() const {
    if (!_ready) return false;
    return IMU.accelerationAvailable() && IMU.gyroscopeAvailable();
}

DataPoint IMUReader::read() {
    DataPoint dp = {};
    dp.ts = millis();
    IMU.readAcceleration(dp.acc_x, dp.acc_y, dp.acc_z);
    IMU.readGyroscope(dp.gyr_x, dp.gyr_y, dp.gyr_z);
    return dp;
}