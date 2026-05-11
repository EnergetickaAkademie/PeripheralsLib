#ifndef SIMPLERGB_H
#define SIMPLERGB_H

#include "Peripheral.h"
#include <Arduino.h>

#ifdef CH32V003
class SimpleRGB : public Peripheral {
public:
	SimpleRGB(uint8_t pin);
	void update() override;
	void setColor(uint8_t r, uint8_t g, uint8_t b);

private:
	uint8_t _pin;
	uint8_t _r, _g, _b;
	bool _colorChanged;
	
	volatile uint32_t* _bshr;
	uint32_t _set_mask;
	uint32_t _clr_mask;
};
#endif // CH32V003

#endif // SIMPLERGB_H