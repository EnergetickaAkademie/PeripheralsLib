#include "encoder.h"
#include <Arduino.h>

Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint8_t pinSW, int16_t minVal, int16_t maxVal, int16_t steps_per_click)
	: _rotary(pinA, pinB, steps_per_click, minVal, maxVal),
	  _button(pinSW),
	  _currentPosition(minVal),
	  _buttonPressed(false)
{
	_rotary.resetPosition(_currentPosition);
	
	// Use lambda functions to capture 'this' pointer for instance-specific callbacks
	_rotary.setChangedHandler([this](ESPRotary &r) {
		this->handleRotation(r);
	});
	
	_button.begin(_button.getPin());
	_button.setClickHandler([this](Button2& b) {
		this->handleButton(b);
	});
	_button.setLongClickHandler([this](Button2& b) {
		this->handleButton(b);
	});

	_rotary.enableSpeedup(true); 
	_rotary.setSpeedupIncrement(5); 
	_rotary.setSpeedupInterval(75); 
}

void Encoder::update() {
	_rotary.loop();
	_button.loop();
}

void Encoder::handleRotation(ESPRotary &r) {
	_currentPosition = r.getPosition();
}

void Encoder::handleButton(Button2& b) {
	_buttonPressed = true;
}

int16_t Encoder::getValue() {
	return _currentPosition;
}

void Encoder::setValue(int16_t value) {
	_rotary.resetPosition(value);
	_currentPosition = _rotary.getPosition();
}

bool Encoder::isButtonPressed() {
	if (_buttonPressed) {
		_buttonPressed = false;
		return true;
	}
	return false;
}