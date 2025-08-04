#include "atomizer.h"

Atomizer::Atomizer(uint8_t pin) 
    : _pin(pin), _state(IDLE), _stateStartTime(0), 
      _pinState(HIGH), _pinStateChanged(true), 
      _isOn(false), _pulsesToSend(0) {
    pinMode(_pin, OUTPUT);
    // Set initial pin state to HIGH (idle)
    digitalWrite(_pin, _pinState);
}

void Atomizer::update() {
    // Handle pin state changes first
    if (_pinStateChanged) {
        digitalWrite(_pin, _pinState);
        _pinStateChanged = false;
    }
    
    if (_state == IDLE) {
        return; // Nothing to do
    }

    unsigned long currentTime = millis();
    unsigned long elapsed = currentTime - _stateStartTime;

    if (elapsed >= PULSE_DURATION) {
        _stateStartTime = currentTime; // Reset timer for the next state

        switch (_state) {
            case PULSE_START:
                // Transition from initial HIGH to LOW
                _pinState = LOW;
                _pinStateChanged = true;
                _state = PULSE_LOW;
                break;
                
            case PULSE_LOW:
                // Transition from LOW back to HIGH
                _pinState = HIGH;
                _pinStateChanged = true;
                _state = PULSE_END;
                break;
                
            case PULSE_END:
                // Pulse finished, decrement count
                _pulsesToSend--;
                
                if (_pulsesToSend > 0) {
                    // If more pulses are needed, start the next one
                    _state = PULSE_START;
                } else {
                    // Sequence complete, return to idle
                    _state = IDLE;
                }
                break;

            case IDLE:
                // Should not happen here, but for safety
                break;
        }
    }
}

void Atomizer::toggle() {
    if (isActive()) {
        return; // Don't start a new sequence if one is already running
    }

    _isOn = !_isOn; // Toggle the logical state

    if (_isOn) {
        _pulsesToSend = 1; // 1 pulse to turn ON
    } else {
        _pulsesToSend = 2; // 2 pulses to turn OFF
    }

    // Start the first pulse
    _state = PULSE_START;
    _stateStartTime = millis();
}

bool Atomizer::isActive() const {
    return _state != IDLE;
}