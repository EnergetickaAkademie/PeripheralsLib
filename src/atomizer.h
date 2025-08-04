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
     * Note: Turning the atomizer OFF requires two consecutive toggle operations.
     */
    void toggle();

    /**
     * @brief Check if the atomizer is currently executing a pulse sequence.
     * @return true if a pulse sequence is in progress, false otherwise.
     */
    bool isActive() const;

    /**
     * @brief Check if the atomizer is currently ON.
     * @return true if atomizer is ON, false if OFF.
     */
    bool isAtomizerOn() const;

private:
    enum PulseState {
        IDLE,           // Not executing any sequence
        FIRST_HIGH,     // First HIGH state (50ms)
        LOW_PULSE,      // LOW state (50ms)  
        FINAL_HIGH      // Final HIGH state (50ms)
    };

    enum AtomizerState {
        ATOMIZER_OFF,           // Atomizer is OFF
        ATOMIZER_ON,            // Atomizer is ON
        ATOMIZER_TURNING_OFF    // Atomizer received first OFF pulse, needs one more
    };

    uint8_t _pin;
    PulseState _state;
    AtomizerState _atomizerState;
    PulseState _pendingState;  // For handling requests during active sequence
    unsigned long _stateStartTime;
    bool _pinState;            // Current pin state
    bool _pinStateChanged;     // Flag to indicate pin state needs updating
    static const unsigned long PULSE_DURATION = 150; // 150ms for each state
};

#endif // ATOMIZER_H