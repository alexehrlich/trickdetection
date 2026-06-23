#include "BLEController.hpp"

BLEController::BLEController(const char* deviceName)
    : _deviceName(deviceName),
      _service(SERVICE_UUID),
      _trickChar(TRICK_CHAR_UUID, BLEWrite, 64),
      _resultChar(RESULT_CHAR_UUID, BLEWrite, 16),
      _trickReceived(false),
      _resultReceived(false),
      _lastTimestamp(0)
{}

bool BLEController::begin() {
    if (!BLE.begin()) {
        Serial.println("BLE init failed");
        return false;
    }

    BLE.setLocalName(_deviceName);
    BLE.setAdvertisedService(_service);
    _service.addCharacteristic(_trickChar);
    _service.addCharacteristic(_resultChar);
    BLE.addService(_service);
    BLE.advertise();

    Serial.println("BLE ready");
    return true;
}

void BLEController::poll() {
    BLE.poll();

    if (_trickChar.written()) {
        String val     = _trickChar.value();
        _lastTrick     = Measurement::parseTrick(val);
        _lastTimestamp = millis();
        _trickReceived = true;
        Serial.println("Trick received: " + val);
    }

    if (_resultChar.written()) {
        String val      = _resultChar.value();
        _lastResult     = Measurement::parseResult(val);
        _resultReceived = true;
        Serial.println("Result received: " + val);
    }
}

bool          BLEController::trickReceived()     const { return _trickReceived; }
bool          BLEController::resultReceived()    const { return _resultReceived; }
Trick         BLEController::getLastTrick()      const { return _lastTrick; }
Result        BLEController::getLastResult()     const { return _lastResult; }
unsigned long BLEController::getLastTimestamp()  const { return _lastTimestamp; }

void BLEController::clearTrick()  { _trickReceived  = false; }
void BLEController::clearResult() { _resultReceived = false; }