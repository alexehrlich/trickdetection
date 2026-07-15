#include "DataPersistor.hpp"

DataPersistor::DataPersistor(int csPin)
    : _csPin(csPin), _ready(false), _writeCount(0)
{
    _currentFilename[0] = '\0';
}

bool DataPersistor::begin() {
    _ready = SD.begin(_csPin);
    if (!_ready) Serial.println("SD init failed");
    return _ready;
}

void DataPersistor::handleSerial() {
    if (!Serial.available()) return;

    closeSession();

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "list") {
        File root = SD.open("/");
        while (true) {
            File entry = root.openNextFile();
            if (!entry) break;
            if (!entry.isDirectory()) {
                Serial.println(entry.name());
            }
            entry.close();
        }
        root.close();
        Serial.println("EOF");
        return;
    }

    if (cmd.startsWith("get:")) {
        String filename = cmd.substring(4);
        File f = SD.open(filename.c_str(), FILE_READ);
        if (!f) {
            Serial.println("ERROR");
            return;
        }
        Serial.println("SIZE:" + String(f.size()));
        uint8_t buf[512];
        int n;
        while ((n = f.read(buf, sizeof(buf))) > 0) {
            Serial.write(buf, n);
        }
        f.close();
        Serial.println("\nEOF");
        return;
    }

    if (cmd.startsWith("delete:")) {
        String filename = cmd.substring(7);
        if (filename.equalsIgnoreCase(_currentFilename)) {
            Serial.println("ERROR: file in use");
            return;
        }
        if (SD.remove(filename.c_str())) {
            Serial.println("DELETED");
        } else {
            Serial.println("ERROR");
        }
        return;
    }
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

    strncpy(_currentFilename, filename, sizeof(_currentFilename));
    _currentFilename[sizeof(_currentFilename) - 1] = '\0';

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
        _currentFilename[0] = '\0';
    }
}

bool DataPersistor::isReady() const { return _ready; }

void DataPersistor::flush() { _file.flush(); }