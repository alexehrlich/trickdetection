#ifndef DATAPERSISTOR_HPP
#define DATAPERSISTOR_HPP

#include <SD.h>
#include "Measurement.hpp"
#include <string>

class DataPersistor {
public:
    DataPersistor(int csPin);

    bool begin();
    bool openSession(String sessionName);
    void writeMeasurementHeader(Trick trick, unsigned long start);
    void writeDataPoint(DataPoint dp);
    void writeMeasurementFooter(Result result);
    void closeSession();
    void handleSerial();

    bool isReady() const;

private:
    int  _csPin;
    File _file;
    bool _ready;
    int  _writeCount;
    char _currentFilename[24];

    void flush();
};

#endif