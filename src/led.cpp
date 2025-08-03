#include "led.h"

LED::LED(int pin) 
	: _pin(pin), _isBlinking(false), _ledState(LOW), _blinkInterval(0), _lastToggleTime(0), _brightness(255), _stateChanged(true) {

	pinMode(_pin, OUTPUT);
	// Don't set pin state here, let update() handle it
}

void LED::setBrightness(uint8_t brightness) {
	_brightness = brightness;
	_stateChanged = true; // Mark for update in next cycle
}

void LED::on() {
	_isBlinking = false;
	_ledState = HIGH;
	_stateChanged = true;
}

void LED::off() {
	_isBlinking = false;
	_ledState = LOW;
	_stateChanged = true;
}

void LED::startBlink(unsigned long interval) {
	_blinkInterval = interval;
	_isBlinking = true;
	_lastToggleTime = millis();
	_stateChanged = true;
}

void LED::stopBlink() {
	_isBlinking = false;
	_ledState = LOW;
	_stateChanged = true;
}

void LED::update() {
	// Handle blinking logic
	if (_isBlinking && (millis() - _lastToggleTime >= _blinkInterval)) {
		_ledState = !_ledState;
		_stateChanged = true;
		_lastToggleTime = millis();
	}
	
	// Handle pin state updates
	if (_stateChanged) {
		_stateChanged = false;
		if (!_isBlinking && _ledState == HIGH) {
			analogWrite(_pin, _brightness);
		} else if (!_isBlinking && _ledState == LOW) {
			analogWrite(_pin, 0);
		} else if (_isBlinking) {
			analogWrite(_pin, _ledState ? _brightness : 0);
		}
	}
}

void LED::setState(bool state) {
	if (state) {
		on();
	} else {
		off();
	}
}