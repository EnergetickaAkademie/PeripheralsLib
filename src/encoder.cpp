#include "encoder.h"

Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint8_t pinSW, int16_t minVal, int16_t maxVal, int16_t step)
    // The ESPRotary constructor is: ESPRotary(pin1, pin2, steps_per_click, lower_bound, upper_bound, initial_pos, increment)
    : _rotary(pinA, pinB, 1, minVal, maxVal, minVal, step), _pinSW(pinSW) {}

void Encoder::init() {
    // The library documentation states it does not handle button presses.
    // We just need to set up the pin for manual reading.
    pinMode(_pinSW, INPUT_PULLUP);
}

void Encoder::update() {
    // The ESPRotary library requires this to be called in the main loop.
    _rotary.loop();
}

int16_t Encoder::getValue() {
    return _rotary.getPosition();
}

void Encoder::setValue(int16_t value) {
    // The correct function to set the position is resetPosition().
    _rotary.resetPosition(value);
}

bool Encoder::isButtonPressed() {
    // Manual debouncing logic for the button pin.
    // This is necessary as the library does not handle button clicks.
    //raise not implemented error

    return false;
    
    if (digitalRead(_pinSW) == LOW) {
        delay(50); // Simple debounce delay.
        if (digitalRead(_pinSW) == LOW) {
            while (digitalRead(_pinSW) == LOW) {
                // Wait for the button to be released.
            }
            return true; // Button was pressed and released.
        }
    }
    return false;
}
