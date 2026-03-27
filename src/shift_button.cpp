#include "shift_button.h"

ShiftButton::ShiftButton(uint8_t register_index, uint8_t bit_position, bool active_high)
    : _register_index(register_index), _bit_position(bit_position), _active_high(active_high),
      _pressed(false), _pressed_event(false), _released_event(false) {
}

void ShiftButton::update(const uint8_t* current_state, const uint8_t* previous_state, uint8_t num_registers) {
    (void)previous_state;

    if (_register_index >= num_registers || _bit_position > 7 || !current_state) {
        return;
    }

    bool current_pressed = get_button_state(current_state);

    if (current_pressed != _pressed) {
        if (current_pressed) {
            _pressed_event = true;
            if (_press_callback) {
                _press_callback();
            }
        } else {
            _released_event = true;
            if (_release_callback) {
                _release_callback();
            }
        }

        if (_state_callback) {
            _state_callback(current_pressed);
        }

        _pressed = current_pressed;
    }
}

bool ShiftButton::was_pressed() {
    bool was = _pressed_event;
    _pressed_event = false;
    return was;
}

bool ShiftButton::was_released() {
    bool was = _released_event;
    _released_event = false;
    return was;
}

bool ShiftButton::get_button_state(const uint8_t* chain_state) const {
    bool raw_state = ((chain_state[_register_index] >> (7 - _bit_position)) & 0x01) != 0;
    return _active_high ? raw_state : !raw_state;
}
