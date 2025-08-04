#ifndef PERIPHERAL_FACTORY_H
#define PERIPHERAL_FACTORY_H

#include <vector>

#include "Peripheral.h"
// Include all concrete peripheral headers so the factory can create them.
#include "led.h"
#ifdef ESP8266
#include "motor.h"
#endif
#include "oled.h"
#ifndef ESP8266
#include "encoder.h"
#endif
#include "rgbled.h"
#include "buzzer.h"
#include "shift_register_chain.h"
#include "segment_display.h"
#include "bargraph.h"
#include "liquid_crystal.h"
#include "button.h"
#include "ledbutton.h"
#include "periodic.h"
#include "atomizer.h"

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
#ifdef ESP8266
    Motor* createMotor(int pinIA, int pinIB, int pwmFreq = 1000);
#endif
    OLEDDisplay* createOLED(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin = -1);
#ifndef ESP8266
    Encoder* createEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinSW, int16_t minVal = 0, int16_t maxVal = 100, int16_t step = 1,
                                bool enable_speedup = true, unsigned int speedup_increment = 5, unsigned int speedup_interval = 75);
#endif
    RGBLED* createRGBLED(uint8_t pin, uint16_t numPixels = 1, neoPixelType type = NEO_GRB + NEO_KHZ800);
    Buzzer* createBuzzer(uint8_t pin);
    ShiftRegisterChain* createShiftRegisterChain(uint8_t latchPin, uint8_t dataPin, uint8_t clockPin);
    LiquidCrystal* createLiquidCrystal(uint8_t address, uint8_t cols, uint8_t rows);
    Button* createButton(uint8_t pin);
    LEDButton* createLEDButton(uint8_t buttonPin, uint8_t ledPin);
    Periodic* createPeriodic(unsigned long interval, std::function<void()> callback);
    Atomizer* createAtomizer(uint8_t pin);

    // --- Factory Methods for Shift Register Devices ---
    Bargraph* createBargraph(ShiftRegisterChain* chain, uint8_t numLeds = 16);
    SegmentDisplay* createSegmentDisplay(ShiftRegisterChain* chain, uint8_t numDigits = 4);

    /**
     * @brief Calls the update() method on all registered peripherals.
     */
    void update();

private:
    void add(Peripheral* peripheral); // Private, used internally by `create` methods

    std::vector<Peripheral*> _peripherals;
};

#endif // PERIPHERAL_FACTORY_H