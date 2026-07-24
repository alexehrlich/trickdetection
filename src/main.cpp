#include <Arduino.h>
#include "BLEController.hpp"
#include "DataPersistor.hpp"
#include "IMUReader.hpp"
#include "Measurement.hpp"

const int CS_PIN = 10;

BLEController ble("sk8ordie");
DataPersistor persistor(CS_PIN);
IMUReader     imu;
Measurement*  current = nullptr;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    if (!imu.begin()) {
        Serial.println("IMU init failed — halting");
        while (1);
    }
    Serial.println("IMU init successful");

    if (!ble.begin()) {
        Serial.println("BLE init failed — halting");
        while (1);
    }
    Serial.println("BLE init successful");

    if (!persistor.begin()) {
        Serial.println("SD init failed — halting");
        while (1);
    }

    Serial.println("All systems ready");
}

void loop() {

    //if USB-cable is connected to read the data
    persistor.handleSerial();

    ble.poll();

    if (ble.sessionReceived()) {
        persistor.openSession(ble.getSessionName());
        ble.clearSession();
    }

    
    if (ble.trickReceived()) {
        if (current != nullptr) {
            delete current;
            current = nullptr;
        }
        current = new Measurement(ble.getLastTrick(), ble.getLastTimestamp());
        persistor.writeMeasurementHeader(current->getTrick(), current->getStart());
        ble.clearTrick();
        Serial.println("Measurement started");
    }

    if (current != nullptr && imu.isAvailable()) {
        DataPoint dp = imu.read();
        persistor.writeDataPoint(dp);
    }

    if (ble.resultReceived() && current != nullptr) {
        current->setResult(ble.getLastResult());
        persistor.writeMeasurementFooter(current->getResult());
        delete current;
        current = nullptr;
        ble.clearResult();
        Serial.println("Measurement saved");
    }
}