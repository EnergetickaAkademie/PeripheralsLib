#include "PeripheralFactory.h"
#include <Arduino.h>

PeripheralFactory::PeripheralFactory() {
	_peripheralCount = 0;
}

PeripheralFactory::~PeripheralFactory() {
	for (int i = 0; i < _peripheralCount; i++) {
		delete _peripherals[i];
		_peripherals[i] = nullptr;
	}
}

void PeripheralFactory::add(Peripheral* peripheral) {
	if (_peripheralCount < MAX_PERIPHERALS) {
		_peripherals[_peripheralCount] = peripheral;
		_peripheralCount++;
	} else {
		Serial.println(F("Error: PeripheralFactory is full. Cannot add more peripherals."));
		delete peripheral;
	}
}

LED* PeripheralFactory::createLed(int pin) {
	LED* led = new LED(pin);
	add(led);
	return led;
}

Motor* PeripheralFactory::createMotor(int pinIA, int pinIB) {
	Motor* motor = new Motor(pinIA, pinIB);
	add(motor);
	return motor;
}

OLEDDisplay* PeripheralFactory::createOLED(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin) {
	OLEDDisplay* oled = new OLEDDisplay(w, h, twi, rst_pin);
	add(oled);
	return oled;
}

Encoder* PeripheralFactory::createEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinSW, int16_t minVal, int16_t maxVal, int16_t step) {
	Encoder* encoder = new Encoder(pinA, pinB, pinSW, minVal, maxVal, step);
	add(encoder);
	return encoder;
}

RGBLED* PeripheralFactory::createRGBLED(uint8_t pin, uint16_t numPixels, neoPixelType type) {
	RGBLED* rgbled = new RGBLED(pin, numPixels, type);
	add(rgbled);
	return rgbled;
}

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

LiquidCrystal* PeripheralFactory::createLiquidCrystal(uint8_t address, uint8_t cols, uint8_t rows) {
	LiquidCrystal* lcd = new LiquidCrystal(address, cols, rows);
	add(lcd);
	return lcd;
}

void PeripheralFactory::update() {
	for (int i = 0; i < _peripheralCount; i++) {
		if (_peripherals[i] != nullptr) {
			_peripherals[i]->update();
		}
	}
}