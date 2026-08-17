#include "shift_encoder.h"
#include <Arduino.h>

ShiftEncoder::ShiftEncoder(uint8_t register_index, uint8_t bit_position, 
                           int32_t min_value, int32_t max_value, int32_t step)
    : _register_index(register_index), _bit_position(bit_position),
      _min_value(min_value), _max_value(max_value), _step(step),
      _current_value(min_value), _previous_state(0), _initialized(false) {
}

void ShiftEncoder::update(const uint8_t* current_state, const uint8_t* previous_state, uint8_t num_registers) {
    if (_register_index >= num_registers) {
        return; // Safety check
    }
    
    uint8_t current_encoder_state = get_encoder_state(current_state);

    // Establish the electrical resting state without reporting a false step.
    if (!_initialized) {
        _previous_state = current_encoder_state;
        _initialized = true;
        return;
    }

    uint8_t previous_encoder_state = _previous_state;

    // Count only the rising edge of channel A. A complete mechanical detent
    // produces both a rising and a falling edge, so counting both doubles it.
    if (current_encoder_state != previous_encoder_state) {
        bool currA = (current_encoder_state >> 1) & 0x01;
        bool currB = current_encoder_state & 0x01;
        bool prevA = (previous_encoder_state >> 1) & 0x01;

        if (!prevA && currA) {
            handle_rotation(currA != currB ? 1 : -1);
        }
    }

    _previous_state = current_encoder_state;
}

void ShiftEncoder::set_value(int32_t value) {
    _current_value = value;
    
    // Constrain to min/max range
    if (_current_value < _min_value) {
        _current_value = _min_value;
    } else if (_current_value > _max_value) {
        _current_value = _max_value;
    }
    
    // Call callback if set
    if (_callback) {
        _callback(_current_value);
    }
}

uint8_t ShiftEncoder::get_encoder_state(const uint8_t* chain_state) {
    // Extract the two bits for this encoder (A and B channels)
    uint8_t state = 0;
    state |= ((chain_state[_register_index] >> (7 - _bit_position)) & 0x01) << 1; // A channel
    state |= ((chain_state[_register_index] >> (7 - (_bit_position + 1))) & 0x01); // B channel
    
    return state;
}

void ShiftEncoder::handle_rotation(int8_t direction) {
    int32_t new_value = _current_value + (direction * _step);
    
    // Constrain to min/max range
    if (new_value < _min_value) {
        new_value = _min_value;
    } else if (new_value > _max_value) {
        new_value = _max_value;
    }
    
    // Only update and callback if value actually changed
    if (new_value != _current_value) {
        _current_value = new_value;
        
        if (_callback) {
            _callback(_current_value);
        }
    }
}