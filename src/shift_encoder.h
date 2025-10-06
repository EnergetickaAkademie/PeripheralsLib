#ifndef SHIFT_ENCODER_H
#define SHIFT_ENCODER_H

#include "input_shift_register_device.h"
#include <functional>

// Represents a single rotary encoder connected via shift register
class ShiftEncoder : public InputShiftRegisterDevice {
public:
    typedef std::function<void(int32_t value)> ValueCallback;
    
    ShiftEncoder(uint8_t register_index, uint8_t bit_position, 
                 int32_t min_value = 0, int32_t max_value = 100, int32_t step = 1);
    
    void update(const uint8_t* current_state, const uint8_t* previous_state, uint8_t num_registers) override;
    
    // Get/set current value
    int32_t get_value() const { return _current_value; }
    void set_value(int32_t value);
    
    // Set callback for value changes
    void set_callback(ValueCallback callback) { _callback = callback; }
    
    // Get encoder position in the shift register
    uint8_t get_register_index() const { return _register_index; }
    uint8_t get_bit_position() const { return _bit_position; }

private:
    uint8_t _register_index;
    uint8_t _bit_position;
    int32_t _min_value;
    int32_t _max_value;
    int32_t _step;
    int32_t _current_value;
    uint8_t _previous_state;
    ValueCallback _callback;
    
    uint8_t get_encoder_state(const uint8_t* chain_state);
    void handle_rotation(int8_t direction);
};

#endif // SHIFT_ENCODER_H