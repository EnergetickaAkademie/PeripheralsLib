#include "atomizer.h"

Atomizer::Atomizer(uint8_t pin) 
    : _pin(pin), _state(IDLE), _atomizerState(ATOMIZER_OFF), _pendingState(IDLE), _stateStartTime(0), _pinState(HIGH), _pinStateChanged(true) {
    pinMode(_pin, OUTPUT);
    // Don't set pin state here, let update() handle it
}

void Atomizer::update() {
    // Handle pin state changes
    if (_pinStateChanged) {
        digitalWrite(_pin, _pinState);
        _pinStateChanged = false;
    }
    
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
                _pinState = LOW;
                _pinStateChanged = true;
                _state = LOW_PULSE;
                _stateStartTime = currentTime;
                break;
                
            case LOW_PULSE:
                // Transition from LOW to HIGH
                _pinState = HIGH;
                _pinStateChanged = true;
                _state = FINAL_HIGH;
                _stateStartTime = currentTime;
                break;
                
            case FINAL_HIGH:
                // Sequence complete, return to idle and update atomizer state
                _state = IDLE;
                
                // Update atomizer state based on current state
                switch (_atomizerState) {
                    case ATOMIZER_OFF:
                        _atomizerState = ATOMIZER_ON;
                        break;
                    case ATOMIZER_ON:
                        _atomizerState = ATOMIZER_TURNING_OFF;
                        break;
                    case ATOMIZER_TURNING_OFF:
                        _atomizerState = ATOMIZER_OFF;
                        break;
                }
                break;
                
            case IDLE:
                // Should not reach here, but just in case
                break;
        }
    }
}

void Atomizer::toggle() {
    if (_state == IDLE) {
        // Start the pulse sequence - only set flags, no pin operations
        _pinState = HIGH;
        _pinStateChanged = true;
        _state = FIRST_HIGH;
        _stateStartTime = millis();
    }
    // If already active, ignore the request (or queue it if needed)
}

bool Atomizer::isActive() const {
    return _state != IDLE;
}

bool Atomizer::isAtomizerOn() const {
    return _atomizerState == ATOMIZER_ON;
}