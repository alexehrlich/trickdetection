#ifndef BLECONTROLLER_HPP
#define BLECONTROLLER_HPP

#include <ArduinoBLE.h>
#include "Measurement.hpp"

#define SERVICE_UUID     "12345678-1234-1234-1234-123456789abc"
#define TRICK_CHAR_UUID  "12345678-1234-1234-1234-123456789abd"
#define RESULT_CHAR_UUID "12345678-1234-1234-1234-123456789abe"

class BLEController {
public:
    BLEController(const char* deviceName);

    bool begin();
    void poll();

    bool trickReceived()  const;
    bool resultReceived() const;

    Trick         getLastTrick()      const;
    Result        getLastResult()     const;
    unsigned long getLastTimestamp()  const;

    void clearTrick();
    void clearResult();

private:
    const char*             _deviceName;
    BLEService              _service;
    BLEStringCharacteristic _trickChar;
    BLEStringCharacteristic _resultChar;

    bool          _trickReceived;
    bool          _resultReceived;
    Trick         _lastTrick;
    Result        _lastResult;
    unsigned long _lastTimestamp;
};

#endif