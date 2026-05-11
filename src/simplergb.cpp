//adapted from https://github.com/bigjosh/SimpleNeoPixelDemo/blob/master/SimpleNeopixelDemo/SimpleNeopixelDemo.ino

#ifdef CH32V003

#include "simplergb.h"

static inline void ws2812_send_bit(uint8_t bitVal, volatile uint32_t *bshr, uint32_t set_mask, uint32_t clr_mask) {
	if (bitVal) {
		__asm__ volatile (
			"sw %[set], 0(%[bshr]) \n\t"
			".rept 36 \n\t nop \n\t .endr \n\t" 
			"sw %[clr], 0(%[bshr]) \n\t"
			".rept 20 \n\t nop \n\t .endr \n\t"
			:: [set] "r" (set_mask), [clr] "r" (clr_mask), [bshr] "r" (bshr)
		);
	} else {
		__asm__ volatile (
			"sw %[set], 0(%[bshr]) \n\t"
			".rept 17 \n\t nop \n\t .endr \n\t"
			"sw %[clr], 0(%[bshr]) \n\t"
			".rept 39 \n\t nop \n\t .endr \n\t"
			:: [set] "r" (set_mask), [clr] "r" (clr_mask), [bshr] "r" (bshr)
		);
	}
}

static inline void ws2812_send_byte(uint8_t byte, volatile uint32_t *bshr, uint32_t set_mask, uint32_t clr_mask) {
	for(int8_t i = 7; i >= 0; i--) {
		ws2812_send_bit((byte >> i) & 1, bshr, set_mask, clr_mask);
	}
}

SimpleRGB::SimpleRGB(uint8_t pin, uint16_t numPixels) : _pin(pin), _r(0), _g(0), _b(0), _colorChanged(true) {
	pinMode(_pin, OUTPUT);
	
	GPIO_TypeDef *port = (GPIO_TypeDef *)digitalPinToPort(_pin);
	uint16_t pin_bm = digitalPinToBitMask(_pin);
	
	_bshr = &(port->BSHR);
	_set_mask = pin_bm;
	_clr_mask = (uint32_t)pin_bm << 16;
}

void SimpleRGB::update() {
	if (_colorChanged) {
		__asm__ volatile("csrci mstatus, 8");

		ws2812_send_byte(_g, _bshr, _set_mask, _clr_mask);
		ws2812_send_byte(_r, _bshr, _set_mask, _clr_mask);
		ws2812_send_byte(_b, _bshr, _set_mask, _clr_mask);

		__asm__ volatile("csrsi mstatus, 8");
		
		_colorChanged = false;
	}
}

void SimpleRGB::setColor(uint8_t r, uint8_t g, uint8_t b) {
	_r = r; 
	_g = g; 
	_b = b;
	_colorChanged = true;
}

#endif