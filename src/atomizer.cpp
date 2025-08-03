#include "atomizer.h"

Atomizer::Atomizer(uint8_t pin) 
    : _pin(pin), _state(IDLE), _stateStartTime(0) {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH); // Default state is HIGH (atomizer off)
}

void Atomizer::update() {
    if (_state == IDLE) {
        return; // Nothing to do when idle
    }

    unsigned long currentTime = millis();
    unsigned long elapsed = currentTime - _stateStartTime;

    // Check if current state duration has elapsed
    if (elapsed >= PULSE_DURATION) {
        switch (_state) {
            case FIRST_HIGH:
                // Transition from HIGH to LOW
                digitalWrite(_pin, LOW);
                _state = LOW_PULSE;
                _stateStartTime = currentTime;
                break;
                
            case LOW_PULSE:
                // Transition from LOW to HIGH
                digitalWrite(_pin, HIGH);
                _state = FINAL_HIGH;
                _stateStartTime = currentTime;
                break;
                
            case FINAL_HIGH:
                // Sequence complete, return to idle
                _state = IDLE;
                break;
                
            case IDLE:
                // Should not reach here, but just in case
                break;
        }
    }
}

void Atomizer::toggle() {
    if (_state == IDLE) {
        // Start the pulse sequence
        digitalWrite(_pin, HIGH);
        _state = FIRST_HIGH;
        _stateStartTime = millis();
    }
    // If already active, ignore the request
}

bool Atomizer::isActive() const {
    return _state != IDLE;
}