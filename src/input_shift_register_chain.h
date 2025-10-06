#ifndef INPUT_SHIFT_REGISTER_CHAIN_H
#define INPUT_SHIFT_REGISTER_CHAIN_H

#include "Peripheral.h"
#include "input_shift_register_device.h"
#include <vector>

class InputShiftRegisterChain : public Peripheral {
public:
    InputShiftRegisterChain(uint8_t load_pin, uint8_t data_pin, uint8_t clock_pin, uint8_t num_registers);
    
    void update() override;
    
    // Add input devices to the chain
    void add_device(InputShiftRegisterDevice* device);
    
    // Read the entire chain (blocking)
    void read_chain();

private:
    uint8_t _load_pin, _data_pin, _clock_pin, _num_registers;
    uint8_t* _current_state;
    uint8_t* _previous_state;
    std::vector<InputShiftRegisterDevice*> _devices;
};

#endif // INPUT_SHIFT_REGISTER_CHAIN_H