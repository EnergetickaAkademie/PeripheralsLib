#include "motor.h"

#ifdef ESP8266

Motor::Motor(int pinIA, int pinIB, int pwmFreq) 
    : _pinIA(pinIA), _pinIB(pinIB), _pwmFreq(pwmFreq), _state(STOPPED), _speed(0), _stateChanged(true) {
	pinMode(_pinIA, OUTPUT);
	pinMode(_pinIB, OUTPUT);
	// Don't set pin states here, let update() handle it
}

void Motor::update() {
    if (!_stateChanged) return;
    
    _stateChanged = false;
    int pwmSpeed = constrain(_speed, 0, 1023);
    
    switch (_state) {
        case FORWARD:
            analogWriteFreq(_pwmFreq);
            analogWrite(_pinIA, pwmSpeed);
            analogWrite(_pinIB, 0);
            break;
        case BACKWARD:
            analogWriteFreq(_pwmFreq);
            analogWrite(_pinIA, 0);
            analogWrite(_pinIB, pwmSpeed);
            break;
        case STOPPED:
        default:
            analogWrite(_pinIA, 0);
            analogWrite(_pinIB, 0);
            break;
    }
}

void Motor::forward(int speed) {
	_speed = speed;
	_state = FORWARD;
	_stateChanged = true;
}

void Motor::backward(int speed) {
	_speed = speed;
	_state = BACKWARD;
	_stateChanged = true;
}

void Motor::stop() {
	_speed = 0;
	_state = STOPPED;
	_stateChanged = true;
}

#endif // ESP8266
