#ifndef BLECONTROLLER_HPP
#define BLECONTROLLER_HPP

#include <ArduinoBLE.h>
#include "Measurement.hpp"

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define TRICK_CHAR_UUID     "12345678-1234-1234-1234-123456789abd"
#define RESULT_CHAR_UUID    "12345678-1234-1234-1234-123456789abe"
#define SESSION_NAME_UUID   "12345678-1234-1234-1234-123456789AC1"
class BLEController {
public:
    BLEController(const char* deviceName);

    bool begin();
    void poll();

    bool trickReceived()  const;
    bool resultReceived() const;
    bool sessionReceived() const;

    Trick         getLastTrick()      const;
    Result        getLastResult()     const;
    String        getSessionName()    const;
    unsigned long getLastTimestamp()  const;

    void clearTrick();
    void clearResult();

private:
    const char*             _deviceName;
    BLEService              _service;
    BLEStringCharacteristic _trickChar;
    BLEStringCharacteristic _resultChar;
    BLEStringCharacteristic _sessionName;

    bool          _trickReceived;
    bool          _resultReceived;
    bool          _sessionNameReceived;
    Trick         _lastTrick;
    Result        _lastResult;
    String        _lastSessionName;
    unsigned long _lastTimestamp;
};

#endif