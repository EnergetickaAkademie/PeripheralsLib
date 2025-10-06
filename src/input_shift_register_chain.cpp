#include "input_shift_register_chain.h"
#include <Arduino.h>

InputShiftRegisterChain::InputShiftRegisterChain(uint8_t load_pin, uint8_t data_pin, uint8_t clock_pin, uint8_t num_registers)
    : _load_pin(load_pin), _data_pin(data_pin), _clock_pin(clock_pin), _num_registers(num_registers) {
    
    pinMode(_load_pin, OUTPUT);
    pinMode(_data_pin, INPUT);
    pinMode(_clock_pin, OUTPUT);
    
    _current_state = new uint8_t[_num_registers];
    _previous_state = new uint8_t[_num_registers];
    
    // Initialize states
    for (int i = 0; i < _num_registers; i++) {
        _current_state[i] = 0;
        _previous_state[i] = 0;
    }
    
    // Set initial pin states
    digitalWrite(_clock_pin, LOW);
    digitalWrite(_load_pin, HIGH);
}

void InputShiftRegisterChain::update() {
    read_chain();
}

void InputShiftRegisterChain::read_chain() {
    // Load parallel data into registers
    digitalWrite(_load_pin, LOW);
    delayMicroseconds(5); // Small pulse for reliable loading
    digitalWrite(_load_pin, HIGH);
    delayMicroseconds(5);
    
    // Read data from all registers in the chain
    for (int reg = 0; reg < _num_registers; reg++) {
        uint8_t data = 0;
        
        for (int i = 0; i < 8; i++) {
            data |= (digitalRead(_data_pin) << (7 - i));
            
            // Pulse clock to shift next bit
            digitalWrite(_clock_pin, HIGH);
            delayMicroseconds(2);
            digitalWrite(_clock_pin, LOW);
            delayMicroseconds(2);
        }
        
        _current_state[reg] = data;
    }
    
    // Update all devices with the new state
    for (InputShiftRegisterDevice* device : _devices) {
        device->update(_current_state, _previous_state, _num_registers);
    }
    
    // Update previous state for next comparison
    for (int i = 0; i < _num_registers; i++) {
        _previous_state[i] = _current_state[i];
    }
}

void InputShiftRegisterChain::add_device(InputShiftRegisterDevice* device) {
    _devices.push_back(device);
}