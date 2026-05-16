#ifndef SEGMENT_DISPLAY_PAIR_H
#define SEGMENT_DISPLAY_PAIR_H

#include <Arduino.h>
#include "segment_display.h"

class SegmentDisplayPair {
public:
	class Half {
	public:
		Half() = default;
		Half(SegmentDisplayPair* parent, bool left);

		void displayNumber(int32_t value, uint8_t decimals = 0) const;
		void clear() const;

	private:
		SegmentDisplayPair* _parent = nullptr;
		bool _left = true;
	};

	SegmentDisplayPair() = default;
	explicit SegmentDisplayPair(SegmentDisplay* display);

	void attach(SegmentDisplay* display);

	Half left();
	Half right();

	SegmentDisplay* getDisplay() const;

private:
	void clearHalf(bool isLeft);
	void setHalf(bool isLeft, int32_t value, uint8_t decimals);
	void render();

	SegmentDisplay* _display = nullptr;
	int32_t _leftValue = 0;
	int32_t _rightValue = 0;
	bool _leftActive = true;
	bool _rightActive = true;
};

#endif // SEGMENT_DISPLAY_PAIR_H
