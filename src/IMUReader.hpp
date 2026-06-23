#ifndef IMUREADER_HPP
#define IMUREADER_HPP

#include <Arduino_BMI270_BMM150.h>
#include "Measurement.hpp"

class IMUReader {
public:
    IMUReader();

    bool begin();
    bool isAvailable() const;
    DataPoint read();

private:
    bool _ready;
};

#endif