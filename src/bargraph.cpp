#include "bargraph.h"
#include <string.h>

Bargraph::Bargraph(uint8_t numLeds) : _numLeds(numLeds), _reversed(false), _enabled(true),
_mode(BargraphMode::NORMAL), _minVal(0), _maxVal(numLeds) {

	_registerCount = (numLeds + 7) / 8;
	_shiftData = new byte[_registerCount];

	for (int i = 0; i < _registerCount; ++i) {
		_shiftData[i] = 0x00;
	}
}

Bargraph::~Bargraph() {
	delete[] _shiftData;
}

void Bargraph::setMode(BargraphMode mode) {
	_mode = mode;
}

void Bargraph::setRange(int minVal, int maxVal) {
	_minVal = minVal;
	_maxVal = maxVal;
}

void Bargraph::setValue(int value) {
	for (int i = 0; i < _registerCount; ++i) {
		_shiftData[i] = 0x00;
	}

	int start_led = 0;
	int end_led = -1; // -1 signifies no LEDs to turn on

	if (_mode == BargraphMode::NORMAL) {
		int ledsToLight = value;

		// Map value if custom range is used
		if (_minVal != 0 || _maxVal != _numLeds) {
			if (_maxVal > _minVal) {
				if (value <= _minVal) ledsToLight = 0;
				else if (value >= _maxVal) ledsToLight = _numLeds;
				else ledsToLight = (value - _minVal) * _numLeds / (_maxVal - _minVal);
			}
		} else {
			// Backward compatible default behavior
			if (ledsToLight < 0) ledsToLight = 0;
			if (ledsToLight > _numLeds) ledsToLight = _numLeds;
		}

		if (ledsToLight > 0) {
			start_led = 0;
			end_led = ledsToLight - 1;
		}
	}
	else if (_mode == BargraphMode::CENTER) {
		int left_center = (_numLeds - 1) / 2;
		int right_center = _numLeds / 2;

		start_led = left_center;
		end_led = right_center;

		if (value > 0) {
			if (value >= _maxVal) {
				end_led = _numLeds - 1;
			} else if (_maxVal > 0) {
				int steps_right = _numLeds - 1 - right_center;
				int extra_leds = (value * steps_right + _maxVal - 1) / _maxVal; // ceil division
				end_led = right_center + extra_leds;
			}
		} else if (value < 0) {
			if (value <= _minVal) {
				start_led = 0;
			} else if (_minVal < 0) {
				int steps_left = left_center;
				int abs_val = -value;
				int abs_min = -_minVal;
				int extra_leds = (abs_val * steps_left + abs_min - 1) / abs_min; // ceil division
				start_led = left_center - extra_leds;
			}
		}
	}

	// Turn on the assigned block of LEDs
	for (int i = start_led; i <= end_led; ++i) {
		int led_index = _reversed ? (_numLeds - 1 - i) : i;

		uint8_t byteIndex = (_registerCount - 1) - (led_index / 8);
		uint8_t bitIndex = led_index % 8;

		if (byteIndex < _registerCount) {
			_shiftData[byteIndex] |= (1 << bitIndex);
		}
	}
}

void Bargraph::setReversed(bool reversed) {
	_reversed = reversed;
}

void Bargraph::setRawData(const byte* data, uint8_t count) {
	if (count > _registerCount) {
		count = _registerCount;
	}
	memcpy(_shiftData, data, count);
}

void Bargraph::setEnabled(bool enabled) {
	_enabled = enabled;
}

bool Bargraph::isEnabled() const {
	return _enabled;
}

const byte* Bargraph::getShiftData() const {
	if (!_enabled) {
		static byte emptyData[16] = {0};
		return emptyData;
	}
	return _shiftData;
}

uint8_t Bargraph::getRegisterCount() const {
	return _registerCount;
}
