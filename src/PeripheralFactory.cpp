#include "PeripheralFactory.h"
#include <Arduino.h>

PeripheralFactory::PeripheralFactory() {

}

PeripheralFactory::~PeripheralFactory() {
	for (auto& peripheral : _peripherals) {
		delete peripheral;
		peripheral = nullptr;
	}
}

void PeripheralFactory::add(Peripheral* peripheral) {
	if (peripheral) {
		_peripherals.push_back(peripheral);
	} else {
		Serial.println(F("Error: Attempted to add a null peripheral."));
	}
}

LED* PeripheralFactory::createLed(int pin) {
	LED* led = new LED(pin);
	add(led);
	return led;
}

Motor* PeripheralFactory::createMotor(int pinIA, int pinIB, int pwmFreq) {
	Motor* motor = new Motor(pinIA, pinIB, pwmFreq);
	add(motor);
	return motor;
}

MotorSinglePin* PeripheralFactory::createMotorSinglePin(int pinA, int pwmFreq) {
	MotorSinglePin* motor = new MotorSinglePin(pinA, pwmFreq);
	add(motor);
	return motor;
}

#if !defined(ESP8266) && !defined(CH32V003)
Encoder* PeripheralFactory::createEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinSW, int16_t minVal, int16_t maxVal, int16_t step,
	bool enable_speedup, unsigned int speedup_increment, unsigned int speedup_interval) {
	Encoder* encoder = new Encoder(pinA, pinB, pinSW, minVal, maxVal, step,
		enable_speedup, speedup_increment, speedup_interval);
	add(encoder);
	return encoder;
}
#endif

#ifndef CH32V003
LiquidCrystal* PeripheralFactory::createLiquidCrystal(uint8_t address, uint8_t cols, uint8_t rows) {
	LiquidCrystal* lcd = new LiquidCrystal(address, cols, rows);
	add(lcd);
	return lcd;
}

OLEDDisplay* PeripheralFactory::createOLED(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin) {
	OLEDDisplay* oled = new OLEDDisplay(w, h, twi, rst_pin);
	add(oled);
	return oled;
}

RGBLED* PeripheralFactory::createRGBLED(uint8_t pin, uint16_t numPixels, neoPixelType type) {
	RGBLED* rgbled = new RGBLED(pin, numPixels, type);
	add(rgbled);
	return rgbled;
}
#endif

Buzzer* PeripheralFactory::createBuzzer(uint8_t pin) {
	Buzzer* buzzer = new Buzzer(pin);
	add(buzzer);
	return buzzer;
}

ShiftRegisterChain* PeripheralFactory::createShiftRegisterChain(uint8_t latchPin, uint8_t dataPin, uint8_t clockPin) {
	ShiftRegisterChain* chain = new ShiftRegisterChain(latchPin, dataPin, clockPin);
	add(chain);
	return chain;
}

Button* PeripheralFactory::createButton(uint8_t pin) {
	Button* button = new Button(pin);
	add(button);
	return button;
}

LEDButton* PeripheralFactory::createLEDButton(uint8_t buttonPin, uint8_t ledPin) {
	LEDButton* ledButton = new LEDButton(buttonPin, ledPin);
	add(ledButton);
	return ledButton;
}

Periodic* PeripheralFactory::createPeriodic(unsigned long interval, std::function<void()> callback) {
	Periodic* periodic = new Periodic(interval, callback);
	add(periodic);
	return periodic;
}

Atomizer* PeripheralFactory::createAtomizer(uint8_t pin) {
	Atomizer* atomizer = new Atomizer(pin);
	add(atomizer);
	return atomizer;
}

// --- Shift Register Device Factory Methods ---

Bargraph* PeripheralFactory::createBargraph(ShiftRegisterChain* chain, uint8_t numLeds) {
	if (!chain) return nullptr;

	Bargraph* bargraph = new Bargraph(numLeds);
	chain->addDevice(bargraph);
	return bargraph;
}

SegmentDisplay* PeripheralFactory::createSegmentDisplay(ShiftRegisterChain* chain, uint8_t numDigits) {
	if (!chain) return nullptr;

	SegmentDisplay* display = new SegmentDisplay(numDigits);
	chain->addDevice(display);
	return display;
}

SegmentDisplayPair PeripheralFactory::createSegmentDisplayPair(ShiftRegisterChain* chain, uint8_t numDigits) {
	SegmentDisplayPair pair;
	if (!chain) {
		return pair;
	}

	SegmentDisplay* display = createSegmentDisplay(chain, numDigits);
	pair.attach(display);
	return pair;
}

void PeripheralFactory::update() {
	for (auto& peripheral : _peripherals) {
		if (peripheral) {
			peripheral->update();
		}
	}
}

InputShiftRegisterChain* PeripheralFactory::createInputShiftRegisterChain(uint8_t load_pin, uint8_t data_pin, uint8_t clock_pin, uint8_t num_registers) {
	InputShiftRegisterChain* chain = new InputShiftRegisterChain(load_pin, data_pin, clock_pin, num_registers);
	add(chain);
	return chain;
}

ShiftEncoder* PeripheralFactory::createShiftEncoder(InputShiftRegisterChain* chain, uint8_t register_index, uint8_t bit_position, 
													int32_t min_value, int32_t max_value, int32_t step) {
	if (!chain) return nullptr;
	
	ShiftEncoder* encoder = new ShiftEncoder(register_index, bit_position, min_value, max_value, step);
	chain->add_device(encoder);
	return encoder;
}

#ifdef CH32V003
	SimpleRGB* PeripheralFactory::createSimpleRGB(uint8_t pin, uint16_t numPixels) {
		SimpleRGB* rgb = new SimpleRGB(pin, numPixels);
		add(rgb);
		return rgb;
	}
#endif

ShiftButton* PeripheralFactory::createShiftButton(InputShiftRegisterChain* chain, uint8_t register_index, uint8_t bit_position, bool active_high) {
	if (!chain) return nullptr;

	ShiftButton* button = new ShiftButton(register_index, bit_position, active_high);
	chain->add_device(button);
	return button;
}
