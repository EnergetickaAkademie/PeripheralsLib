#include "motor_single_pin.h"

MotorSinglePin::MotorSinglePin(int pinA, int pwmFreq) 
	: _pinA(pinA), _pwmFreq(pwmFreq), _state(STOPPED), _speed(0), _previousSpeed(0), _stateChanged(true),
	  _speedupEnabled(false), _speedupMultiplier(1.5f), _speedupDuration(300), _isInSpeedup(false), _speedupStartTime(0) {
	pinMode(_pinA, OUTPUT);
	// Pin B is connected through resistor to ground, so we don't control it
}

void MotorSinglePin::update() {
	if (_isInSpeedup && (millis() - _speedupStartTime >= _speedupDuration)) {
		_isInSpeedup = false;
		_stateChanged = true;
	}

	if (!_stateChanged) return;
	
	_stateChanged = false;
	
	int actualSpeed = _speed;
	if (_isInSpeedup && _speedupEnabled) {
		actualSpeed = constrain((int)(_speed * _speedupMultiplier), 0, 1023);
	}
	
	int pwmSpeed = constrain(actualSpeed, 0, 1023);
	
	switch (_state) {
		case FORWARD:
			#ifdef ESP8266
				analogWriteFreq(_pwmFreq);
			#endif

			#ifdef ESP32
				analogWriteFrequency(_pwmFreq);
			#endif

			analogWrite(_pinA, pwmSpeed);
			break;
			
		case STOPPED:
		default:
			analogWrite(_pinA, 0);
			_isInSpeedup = false;
			break;
	}
}

void MotorSinglePin::forward(int speed) {
	bool speedChanged = (_speed != speed);
	_speed = speed;
	_state = FORWARD;
	_stateChanged = true;
	
	if (_speedupEnabled && speedChanged) {
		_isInSpeedup = true;
		_speedupStartTime = millis();
	}
	_previousSpeed = speed;
}

void MotorSinglePin::stop() {
	_speed = 0;
	_state = STOPPED;
	_stateChanged = true;
	_isInSpeedup = false;
	_previousSpeed = 0;
}

void MotorSinglePin::enableSpeedup(bool enabled) {
	_speedupEnabled = enabled;
}

void MotorSinglePin::setSpeedupConfig(float multiplier, unsigned long duration_ms) {
	_speedupMultiplier = multiplier;
	_speedupDuration = duration_ms;
}

bool MotorSinglePin::isSpeedupActive() const {
	return _isInSpeedup;
}
