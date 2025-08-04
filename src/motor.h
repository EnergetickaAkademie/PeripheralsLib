#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "Peripheral.h"

class Motor : public Peripheral {
public:
	Motor(int pinIA, int pinIB, int pwmFreq = 1000);
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
	int _pwmFreq;
	MotorState _state;
	int _speed;
	bool _stateChanged;
};

#endif // MOTOR_H
