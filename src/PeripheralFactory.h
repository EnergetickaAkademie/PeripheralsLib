#ifndef PERIPHERAL_FACTORY_H
#define PERIPHERAL_FACTORY_H

#include <vector>

#include "Peripheral.h"
// Include all concrete peripheral headers so the factory can create them.
#include "led.h"
#include "motor.h"
#include "motor_single_pin.h"

#if !defined(ESP8266) && !defined(CH32V003)
	#include "encoder.h"
#endif

#include "buzzer.h"
#include "shift_register_chain.h"
#include "segment_display.h"
#include "bargraph.h"

#ifndef CH32V003
	#include "oled.h"
	#include "liquid_crystal.h"
	#include "rgbled.h"
#endif

#ifdef CH32V003
	#include "simplergb.h"
#endif

#include "button.h"
#include "ledbutton.h"
#include "periodic.h"
#include "atomizer.h"
#include "input_shift_register_chain.h"
#include "shift_encoder.h"

/**
 * @brief Manages and creates hardware peripherals.
 * 
 * This class uses a factory pattern to create, manage, initialize, and update
 * all hardware peripherals from a central point.
 */
class PeripheralFactory {
public:
	PeripheralFactory();
	~PeripheralFactory(); // Destructor to clean up dynamically allocated peripherals

	// --- Factory Methods ---
	// Create methods for each peripheral type that return a typed pointer.
	LED* createLed(int pin);
	Motor* createMotor(int pinIA, int pinIB, int pwmFreq = 1000);
	MotorSinglePin* createMotorSinglePin(int pinA, int pwmFreq = 1000);
	
	#if !defined(ESP8266) && !defined(CH32V003)
		Encoder* createEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinSW, int16_t minVal = 0, int16_t maxVal = 100, int16_t step = 1,
		bool enable_speedup = true, unsigned int speedup_increment = 5, unsigned int speedup_interval = 75);
	#endif
		
	#ifndef CH32V003
		LiquidCrystal* createLiquidCrystal(uint8_t address, uint8_t cols, uint8_t rows);
		OLEDDisplay* createOLED(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin = -1);
		RGBLED* createRGBLED(uint8_t pin, uint16_t numPixels = 1, neoPixelType type = NEO_GRB + NEO_KHZ800);
	#endif

	Buzzer* createBuzzer(uint8_t pin);
	ShiftRegisterChain* createShiftRegisterChain(uint8_t latchPin, uint8_t dataPin, uint8_t clockPin);
	Button* createButton(uint8_t pin);
	LEDButton* createLEDButton(uint8_t buttonPin, uint8_t ledPin);
	Periodic* createPeriodic(unsigned long interval, std::function<void()> callback);
	Atomizer* createAtomizer(uint8_t pin);

	// --- Factory Methods for Shift Register Devices ---
	Bargraph* createBargraph(ShiftRegisterChain* chain, uint8_t numLeds = 16);
	SegmentDisplay* createSegmentDisplay(ShiftRegisterChain* chain, uint8_t numDigits = 4);

	InputShiftRegisterChain* createInputShiftRegisterChain(uint8_t load_pin, uint8_t data_pin, uint8_t clock_pin, uint8_t num_registers);
	ShiftEncoder* createShiftEncoder(InputShiftRegisterChain* chain, uint8_t register_index, uint8_t bit_position, 
									int32_t min_value = 0, int32_t max_value = 100, int32_t step = 1);

	#ifdef CH32V003
		SimpleRGB* createSimpleRGB(uint8_t pin, uint16_t numPixels = 1);
	#endif
	/**
	 * @brief Calls the update() method on all registered peripherals.
	 */
	void update();

private:
	void add(Peripheral* peripheral); // Private, used internally by `create` methods

	std::vector<Peripheral*> _peripherals;
};

#endif // PERIPHERAL_FACTORY_H