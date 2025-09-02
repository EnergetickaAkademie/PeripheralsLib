#ifndef MOTOR_SINGLE_PIN_H
#define MOTOR_SINGLE_PIN_H

#include <Arduino.h>
#include "Peripheral.h"

class MotorSinglePin : public Peripheral {
public:
	MotorSinglePin(int pinA, int pwmFreq = 1000);
	void update() override;
	void forward(int speed);
	void stop();

	void enableSpeedup(bool enabled);
	void setSpeedupConfig(float multiplier, unsigned long duration_ms);
	bool isSpeedupActive() const;

private:
	enum MotorState {
		STOPPED,
		FORWARD
	};
	
	int _pinA;
	int _pwmFreq;
	MotorState _state;
	int _speed;
	int _previousSpeed;
	bool _stateChanged;
	bool _speedupEnabled;
	float _speedupMultiplier;
	unsigned long _speedupDuration;
	bool _isInSpeedup;
	unsigned long _speedupStartTime;
};

#endif // MOTOR_SINGLE_PIN_H
