#include "motor.h"

Motor::Motor(int pinIA, int pinIB, int pwmFreq) 
	: _pinIA(pinIA), _pinIB(pinIB), _pwmFreq(pwmFreq), _state(STOPPED), _speed(0), _previousSpeed(0), _stateChanged(true),
	  _speedupEnabled(false), _speedupMultiplier(1.5f), _speedupDuration(300), _isInSpeedup(false), _speedupStartTime(0) {
	pinMode(_pinIA, OUTPUT);
	pinMode(_pinIB, OUTPUT);
	// Don't set pin states here, let update() handle it
}

void Motor::update() {
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

			analogWrite(_pinIA, pwmSpeed);
			analogWrite(_pinIB, 0);
			break;
		case BACKWARD:
			#ifdef ESP8266
				analogWriteFreq(_pwmFreq);
			#endif

			#ifdef ESP32
				analogWriteFrequency(_pwmFreq);
			#endif

			analogWrite(_pinIA, 0);
			analogWrite(_pinIB, pwmSpeed);
			break;
			
		case STOPPED:
		default:
			analogWrite(_pinIA, 0);
			analogWrite(_pinIB, 0);
			_isInSpeedup = false;
			break;
	}
}

void Motor::forward(int speed) {
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

void Motor::backward(int speed) {
	bool speedChanged = (_speed != speed);
	_speed = speed;
	_state = BACKWARD;
	_stateChanged = true;
	
	if (_speedupEnabled && speedChanged) {
		_isInSpeedup = true;
		_speedupStartTime = millis();
	}
	_previousSpeed = speed;
}

void Motor::stop() {
	_speed = 0;
	_state = STOPPED;
	_stateChanged = true;
	_isInSpeedup = false;
	_previousSpeed = 0;
}

void Motor::enableSpeedup(bool enabled) {
	_speedupEnabled = enabled;
}

void Motor::setSpeedupConfig(float multiplier, unsigned long duration_ms) {
	_speedupMultiplier = multiplier;
	_speedupDuration = duration_ms;
}

bool Motor::isSpeedupActive() const {
	return _isInSpeedup;
}