#ifndef ENCODER_H
#define ENCODER_H

#include "Peripheral.h"
#include <ESPRotary.h>

class Encoder : public Peripheral {
public:
    /**
     * @brief Construct a new Encoder object.
     * 
     * @param pinA The pin connected to the encoder's A output.
     * @param pinB The pin connected to the encoder's B output.
     * @param pinSW The pin connected to the encoder's switch (button).
     * @param minVal The minimum value (inclusive).
     * @param maxVal The maximum value (inclusive).
     * @param step The step value for each notch.
     */
    Encoder(uint8_t pinA, uint8_t pinB, uint8_t pinSW, int16_t minVal = 0, int16_t maxVal = 100, int16_t step = 1);
    
    // --- Methods from Peripheral interface ---
    void init() override;
    void update() override;

    // --- Value and State Access ---

    /**
     * @brief Gets the current value of the encoder.
     * @return The current value, constrained within the set range.
     */
    int16_t getValue();

    /**
     * @brief Checks if the encoder's button was pressed since the last check.
     * This is a one-shot read; it resets the pressed state after being called.
     * 
     * @return true if the button was pressed, false otherwise.
     */
    bool isButtonPressed();

    /**
     * @brief Sets the encoder's value directly.
     * 
     * @param value The value to set. It will be constrained within the set range.
     */
    void setValue(int16_t value);

private:
    // The ESPRotary object
    ESPRotary _rotary;
    
    // Pin for the switch
    uint8_t _pinSW;
};

#endif // ENCODER_H
