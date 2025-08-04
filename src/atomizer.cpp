#include "atomizer.h"

Atomizer::Atomizer(uint8_t pin) 
    : _pin(pin), _state(IDLE), _pendingState(IDLE), _stateStartTime(0), _pinState(HIGH), _pinStateChanged(true), _isOn(false) {
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
            case FIRST_PULSE_HIGH:
                // Transition from HIGH to LOW (first pulse)
                _pinState = LOW;
                _pinStateChanged = true;
                _state = FIRST_PULSE_LOW;
                _stateStartTime = currentTime;
                break;
                
            case FIRST_PULSE_LOW:
                // Transition from LOW to HIGH (complete first pulse)
                _pinState = HIGH;
                _pinStateChanged = true;
                _state = FIRST_PULSE_FINAL;
                _stateStartTime = currentTime;
                break;
                
            case FIRST_PULSE_FINAL:
                // First pulse complete
                if (_isOn) {
                    // Need second pulse to turn off
                    _pinState = HIGH;
                    _pinStateChanged = true;
                    _state = SECOND_PULSE_HIGH;
                    _stateStartTime = currentTime;
                } else {
                    // Single pulse complete, atomizer is now on
                    _isOn = true;
                    _state = IDLE;
                }
                break;
                
            case SECOND_PULSE_HIGH:
                // Transition from HIGH to LOW (second pulse)
                _pinState = LOW;
                _pinStateChanged = true;
                _state = SECOND_PULSE_LOW;
                _stateStartTime = currentTime;
                break;
                
            case SECOND_PULSE_LOW:
                // Transition from LOW to HIGH (complete second pulse)
                _pinState = HIGH;
                _pinStateChanged = true;
                _state = SECOND_PULSE_FINAL;
                _stateStartTime = currentTime;
                break;
                
            case SECOND_PULSE_FINAL:
                // Second pulse complete, atomizer is now off
                _isOn = false;
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
        // Start the pulse sequence - only set flags, no pin operations
        _pinState = HIGH;
        _pinStateChanged = true;
        _state = FIRST_PULSE_HIGH;
        _stateStartTime = millis();
    }
    // If already active, ignore the request (or queue it if needed)
}

bool Atomizer::isActive() const {
    return _state != IDLE;
}

bool Atomizer::isOn() const {
    return _isOn;
}