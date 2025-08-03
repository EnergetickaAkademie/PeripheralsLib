#include "rgbled.h"
#include <Arduino.h>

RGBLED::RGBLED(uint8_t pin, uint16_t numPixels, neoPixelType type)
	: _strip(numPixels, pin, type), _brightness(255), _r(0), _g(0), _b(0), 
	  _colorChanged(false), _brightnessChanged(false), _showRequested(true) {
	_strip.begin();
	// Don't call show() here, let update() handle it
}

void RGBLED::update() {
	// Handle brightness changes
	if (_brightnessChanged) {
		_strip.setBrightness(_brightness);
		_brightnessChanged = false;
		_showRequested = true; // Need to show after brightness change
	}
	
	// Handle color changes
	if (_colorChanged) {
		_strip.setPixelColor(0, _strip.Color(_r, _g, _b));
		_colorChanged = false;
		_showRequested = true; // Need to show after color change
	}
	
	// Handle show requests
	if (_showRequested) {
		_strip.show();
		_showRequested = false;
	}
}

void RGBLED::setColor(uint8_t r, uint8_t g, uint8_t b) {
	_r = r; 
	_g = g; 
	_b = b;
	_colorChanged = true;
}

void RGBLED::setBrightness(uint8_t brightness) {
	_brightness = brightness;
	_brightnessChanged = true;
}

void RGBLED::show() {
	_showRequested = true;
}
