#ifndef SEGMENT_DISPLAY_PAIR_H
#define SEGMENT_DISPLAY_PAIR_H

#include <Arduino.h>
#include <cstdio>
#include "segment_display.h"

class SegmentDisplayPair {
public:
	class Half {
	public:
		Half() = default;
		Half(SegmentDisplayPair* parent, bool left) : _parent(parent), _left(left) {}

		void displayNumber(int32_t value, uint8_t decimals = 0) const {
			if (_parent) {
				_parent->setHalf(_left, value, decimals);
			}
		}

	private:
		SegmentDisplayPair* _parent = nullptr;
		bool _left = true;
	};

	SegmentDisplayPair() = default;
	explicit SegmentDisplayPair(SegmentDisplay* display) : _display(display) {}

	void attach(SegmentDisplay* display) {
		_display = display;
	}

	Half left() { return Half(this, true); }
	Half right() { return Half(this, false); }

	SegmentDisplay* getDisplay() const { return _display; }

private:
	void setHalf(bool isLeft, int32_t value, uint8_t decimals) {
		if (isLeft) {
			_leftValue = constrain(value, 0, 9999);
		} else {
			_rightValue = constrain(value, 0, 9999);
		}

		if (!_display) {
			return;
		}

		char buffer[9];
		snprintf(buffer, sizeof(buffer), "%04ld%04ld", static_cast<long>(_leftValue), static_cast<long>(_rightValue));
		_display->displayString(buffer);
		(void)decimals;
	}

	SegmentDisplay* _display = nullptr;
	int32_t _leftValue = 0;
	int32_t _rightValue = 0;
};

#endif // SEGMENT_DISPLAY_PAIR_H
