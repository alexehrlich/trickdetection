#ifndef DATAPERSISTOR_HPP
#define DATAPERSISTOR_HPP

#include <SD.h>
#include "Measurement.hpp"

class DataPersistor {
public:
    DataPersistor(int csPin);

    bool begin();
    bool openSession(unsigned long ts);
    void writeMeasurementHeader(Trick trick, unsigned long start);
    void writeDataPoint(DataPoint dp);
    void writeMeasurementFooter(Result result);
    void closeSession();

    bool isReady() const;

private:
    int  _csPin;
    File _file;
    bool _ready;
    int  _writeCount;

    void flush();
};

#endif