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
     */
    void toggle();

    /**
     * @brief Check if the atomizer is currently executing a pulse sequence.
     * @return true if a pulse sequence is in progress, false otherwise.
     */
    bool isActive() const;

private:
    enum PulseState {
        IDLE,           // Not executing any sequence
        PULSE_START,    // Starting a new pulse
        PULSE_LOW,      // LOW state in a pulse
        PULSE_END       // End of a pulse, returning to HIGH
    };

    uint8_t _pin;
    PulseState _state;
    unsigned long _stateStartTime;
    bool _pinState;            // Current pin state
    bool _pinStateChanged;     // Flag to indicate pin state needs updating
    
    bool _isOn;                // Tracks if the atomizer is logically on or off
    uint8_t _pulsesToSend;     // How many pulses are remaining in the current sequence

    static const unsigned long PULSE_DURATION = 50; // 50ms for each state
};

#endif // ATOMIZER_H