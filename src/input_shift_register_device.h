#ifndef INPUT_SHIFT_REGISTER_DEVICE_H
#define INPUT_SHIFT_REGISTER_DEVICE_H

#include <Arduino.h>

// Abstract base class for input devices connected to shift register chain
class InputShiftRegisterDevice {
public:
    virtual ~InputShiftRegisterDevice() {}
    
    // Called by the chain when new data is available
    virtual void update(const uint8_t* current_state, const uint8_t* previous_state, uint8_t num_registers) = 0;
};

#endif // INPUT_SHIFT_REGISTER_DEVICE_H