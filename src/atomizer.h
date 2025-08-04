#ifndef ATOMIZER_H
#define ATOMIZER_H

#include "Peripheral.h"
#include <Arduino.h>

/**
 * @brief Non-blocking atomizer peripheral for mist spraying devices.
 * 
 * The atomizer is controlled by a specific pulse sequence:
 * HIGH -> 50ms -> LOW -> 50ms -> HIGH -> 50ms
 * This sequence toggles the atomizer state (on/off).
 */
class Atomizer : public Peripheral {
public:
    /**
     * @brief Constructor for Atomizer.
     * @param pin The digital pin connected to the atomizer control.
     */
    Atomizer(uint8_t pin);

    /**
     * @brief Update function called in the main loop for non-blocking operation.
     */
    void update() override;

    /**
     * @brief Trigger the atomizer switching sequence.
     * This starts the non-blocking pulse sequence to toggle the atomizer.
     * Note: Turning off requires two complete pulse sequences.
     */
    void toggle();

    /**
     * @brief Check if the atomizer is currently executing a pulse sequence.
     * @return true if a pulse sequence is in progress, false otherwise.
     */
    bool isActive() const;

    /**
     * @brief Check if the atomizer is currently on or off.
     * @return true if atomizer is on, false if off.
     */
    bool isOn() const;

    /**
     * @brief Check the intended state of the atomizer (changes immediately on toggle).
     * @return true if atomizer should be on, false if should be off.
     */
    bool getTargetState() const;

private:
    enum PulseState {
        IDLE,               // Not executing any sequence
        FIRST_PULSE_HIGH,   // First pulse: HIGH state (150ms)
        FIRST_PULSE_LOW,    // First pulse: LOW state (150ms)  
        FIRST_PULSE_FINAL,  // First pulse: Final HIGH state (150ms)
        SECOND_PULSE_HIGH,  // Second pulse: HIGH state (150ms)
        SECOND_PULSE_LOW,   // Second pulse: LOW state (150ms)
        SECOND_PULSE_FINAL  // Second pulse: Final HIGH state (150ms)
    };

    uint8_t _pin;
    PulseState _state;
    PulseState _pendingState;  // For handling requests during active sequence
    unsigned long _stateStartTime;
    bool _pinState;            // Current pin state
    bool _pinStateChanged;     // Flag to indicate pin state needs updating
    bool _isOn;                // Track atomizer state (on/off)
    bool _targetState;         // Track intended state (changes immediately on toggle)
    static const unsigned long PULSE_DURATION = 150; // 150ms for each state
};

#endif // ATOMIZER_H