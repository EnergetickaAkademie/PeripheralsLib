#ifndef SHIFT_BUTTON_H
#define SHIFT_BUTTON_H

#include "input_shift_register_device.h"
#include <functional>

// Represents a single button connected via input shift register chain.
class ShiftButton : public InputShiftRegisterDevice {
public:
    typedef std::function<void()> Callback;
    typedef std::function<void(bool pressed)> StateCallback;

    ShiftButton(uint8_t register_index, uint8_t bit_position, bool active_high = true);

    void update(const uint8_t* current_state, const uint8_t* previous_state, uint8_t num_registers) override;

    bool is_pressed() const { return _pressed; }
    bool was_pressed();
    bool was_released();

    void set_press_callback(Callback callback) { _press_callback = callback; }
    void set_release_callback(Callback callback) { _release_callback = callback; }
    void set_state_callback(StateCallback callback) { _state_callback = callback; }

    uint8_t get_register_index() const { return _register_index; }
    uint8_t get_bit_position() const { return _bit_position; }

private:
    uint8_t _register_index;
    uint8_t _bit_position;
    bool _active_high;
    bool _pressed;
    bool _pressed_event;
    bool _released_event;
    Callback _press_callback;
    Callback _release_callback;
    StateCallback _state_callback;

    bool get_button_state(const uint8_t* chain_state) const;
};

#endif // SHIFT_BUTTON_H
