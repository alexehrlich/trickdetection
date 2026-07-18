#include "BLEController.hpp"

BLEController::BLEController(const char* deviceName)
    : _deviceName(deviceName),
      _service(SERVICE_UUID),
      _trickChar(TRICK_CHAR_UUID, BLEWrite, 64),
      _resultChar(RESULT_CHAR_UUID, BLEWrite, 16),
      _sessionName(SESSION_NAME_UUID, BLEWrite, 24),
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
    _service.addCharacteristic(_sessionName);
    BLE.addService(_service);
    BLE.advertise();

    Serial.println("BLE ready");
    return true;
}

void BLEController::poll() {
    BLE.poll();

    if (_sessionName.written()){
        _lastSessionName = _sessionName.value();
        _sessionNameReceived = true;
        Serial.println("Session-Name received: " + _lastSessionName);
    }

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
bool          BLEController::sessionReceived()   const { return _sessionNameReceived; }
Trick         BLEController::getLastTrick()      const { return _lastTrick; }
Result        BLEController::getLastResult()     const { return _lastResult; }
String        BLEController::getSessionName()     const { return _lastSessionName; }
unsigned long BLEController::getLastTimestamp()  const { return _lastTimestamp; }

void BLEController::clearTrick()  { _trickReceived  = false; }
void BLEController::clearResult() { _resultReceived = false; }