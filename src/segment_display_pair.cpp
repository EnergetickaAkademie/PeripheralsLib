#include "segment_display_pair.h"
#include <cstdio>

SegmentDisplayPair::Half::Half(SegmentDisplayPair* parent, bool left)
: _parent(parent), _left(left) {}

void SegmentDisplayPair::Half::displayNumber(int32_t value, uint8_t decimals) const {
    if (_parent) {
        _parent->setHalf(_left, value, decimals);
    }
}

void SegmentDisplayPair::Half::clear() const {
    if (_parent) {
        _parent->clearHalf(_left);
    }
}

SegmentDisplayPair::SegmentDisplayPair(SegmentDisplay* display)
: _display(display) {}

void SegmentDisplayPair::attach(SegmentDisplay* display) {
    _display = display;
}

SegmentDisplayPair::Half SegmentDisplayPair::left() {
    return Half(this, true);
}

SegmentDisplayPair::Half SegmentDisplayPair::right() {
    return Half(this, false);
}

SegmentDisplay* SegmentDisplayPair::getDisplay() const {
    return _display;
}

void SegmentDisplayPair::clearHalf(bool isLeft) {
    if (isLeft) {
        _leftActive = false;
    } else {
        _rightActive = false;
    }
    render();
}

void SegmentDisplayPair::setHalf(bool isLeft, int32_t value, uint8_t decimals) {
    if (isLeft) {
        _leftValue = constrain(value, 0, 9999);
        _leftActive = true;
    } else {
        _rightValue = constrain(value, 0, 9999);
        _rightActive = true;
    }
    render();
    (void)decimals;
}

void SegmentDisplayPair::render() {
    if (!_display) {
        return;
    }

    char buffer[9];
    char leftStr[5] = "    ";
    char rightStr[5] = "    ";

    if (_leftActive) {
        snprintf(leftStr, sizeof(leftStr), "%04ld", static_cast<long>(_leftValue));
    }
    if (_rightActive) {
        snprintf(rightStr, sizeof(rightStr), "%04ld", static_cast<long>(_rightValue));
    }

    snprintf(buffer, sizeof(buffer), "%s%s", leftStr, rightStr);
    _display->displayString(buffer);
}
