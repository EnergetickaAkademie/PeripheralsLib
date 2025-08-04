#ifndef MOTOR_H
#define MOTOR_H

#ifdef ESP8266

#include <Arduino.h>
#include "Peripheral.h"

class Motor : public Peripheral {
public:
    Motor(int pinIA, int pinIB);
    void update() override;
    void forward(int speed);
    void backward(int speed);
    void stop();

private:
    enum MotorState {
        STOPPED,
        FORWARD,
        BACKWARD
    };
    
    int _pinIA;
    int _pinIB;
    MotorState _state;
    int _speed;
    bool _stateChanged;
};

#endif // ESP8266

#endif // MOTOR_H
