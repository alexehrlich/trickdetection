#include "DataPersistor.hpp"

DataPersistor::DataPersistor(int csPin)
    : _csPin(csPin), _ready(false), _writeCount(0)
{}

bool DataPersistor::begin() {
    _ready = SD.begin(_csPin);
    if (!_ready) Serial.println("SD init failed");
    return _ready;
}

bool DataPersistor::openSession(unsigned long ts) {
    if (!_ready) return false;

    char filename[24];
    snprintf(filename, sizeof(filename), "ses_%lu.csv", ts);

    _file = SD.open(filename, FILE_WRITE);
    if (!_file) {
        Serial.println("Failed to open session file");
        return false;
    }

    Serial.print("Session opened: ");
    Serial.println(filename);
    return true;
}

void DataPersistor::writeMeasurementHeader(Trick trick, unsigned long start) {
    const char* header = "ts,acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z\r\n";
    char buf[64];
    snprintf(buf, sizeof(buf), "---,%s,%lu\n", trickToString(trick), start);
    _file.write(buf, strlen(buf));
    _file.write(header, strlen(header));
}

void DataPersistor::writeDataPoint(DataPoint dp) {
    char buf[80];
    snprintf(buf, sizeof(buf), "%lu,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
        dp.ts,
        dp.acc_x, dp.acc_y, dp.acc_z,
        dp.gyr_x, dp.gyr_y, dp.gyr_z
    );
    printf(buf);
    _file.write(buf, strlen(buf));

    if (++_writeCount % 50 == 0) flush();
}

void DataPersistor::writeMeasurementFooter(Result result) {
    const char* r = (result == Result::LANDED) ? "landed" : "bailed";
    char buf[24];
    snprintf(buf, sizeof(buf), "result,%s\n", r);
    _file.write(buf, strlen(buf));
    flush();
}

void DataPersistor::closeSession() {
    if (_file) {
        _file.close();
        Serial.println("Session closed");
    }
}

bool DataPersistor::isReady() const { return _ready; }

void DataPersistor::flush() { _file.flush(); }