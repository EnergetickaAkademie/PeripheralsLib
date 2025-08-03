#include "buzzer.h"

Buzzer::Buzzer(uint8_t pin) 
	: _pin(pin), _isBuzzing(false), _pinState(LOW), _stateChanged(true), _startTime(0), _duration(0) {

	pinMode(_pin, OUTPUT);
	// Don't set pin state here, let update() handle it
}

void Buzzer::buzz(unsigned long duration_ms) {
	if (duration_ms > 0) {
		_duration = duration_ms;
		_isBuzzing = true;
		_startTime = millis();
		_pinState = HIGH;
		_stateChanged = true;
	}
}

void Buzzer::update() {
	// Check if the buzzer is active and if the duration has passed
	if (_isBuzzing && (millis() - _startTime >= _duration)) {
		_pinState = LOW;
		_stateChanged = true;
		_isBuzzing = false;
	}
	
	// Handle pin state changes
	if (_stateChanged) {
		digitalWrite(_pin, _pinState);
		_stateChanged = false;
	}
}
