# PeripheralsLib

A library for controlling various peripherals with Arduino / ESP boards.

## Features

This library provides a collection of classes to interface with common hardware peripherals, making it easier to build interactive electronic projects.

The peripherals are created by a single factory class, `PeripheralFactory`, which handles all of the update logic and peripheral instance creation.

## Supported Peripherals

Output Devices:
- [x] LEDs: A driver for simple LEDs, supporting blinking and simple on/off control.
- [x] RGB LEDs (WS2812)
- [x] LiquidCrystal over I2C
- [x] Seven-Segment Displays wit the MAX7219 driver.
- [x] Buzzer
- [x] Motor controlled with H Bridge
- [x] OLED Display
- [x] 7 Segment / Bargraph controlled with 595 shift registers
- [] RFID Reader

Input Devices:
- [x] Rotary Encoders
- [] Sliding potentiometers

Peripherals that will be added support:

## Getting Started

### Prerequisites

This library is designed to be used with the PlatformIO ecosystem. You will need to have PlatformIO Core installed and the following libraries available:
- wayoda/LedControl
- https://github.com/OSSLibraries/Arduino_MFRC522v2.git
- adafruit/Adafruit GFX Library @ ^1.11.9
- adafruit/Adafruit SSD1306 @ ^2.5.9
- adafruit/Adafruit NeoPixel
- https://github.com/LennartHennigs/ESPRotary
- https://github.com/LennartHennigs/Button2
- marcoschwartz/LiquidCrystal_I2C @ ^1.1.2

### Installation

You can either clone this repository into your PlatformIO project's `lib` directory or add it as a dependency in your `platformio.ini` file.

```cpp
#include <PeripheralFactory.h>
```

### Usage

The `PeripheralFactory` class is the main entry point for creating peripheral objects. It simplifies the process of initializing and using the various components.

## Examples

### Basic Usage

The standard framework for using this library is as follows:

```cpp
#include <Arduino.h>
#include <PeripheralFactory.h>

#include <PeripheralFactory.h>

PeripheralFactory factory;

void setup() {
	Serial.begin(115200);

	factory.init(); //calls init() method for every periphery
}

void loop() {
	factory.update(); //updates every periphery periodically (like a blinking LED), to avoid the usage of delay()
}
```

### OLED
```cpp
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET_PIN -1
#define I2C_ADDRESS 0x3C

OLEDDisplay* oled = factory.createOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

oled->clear();
oled->setCursor(0, 0);
oled->setTextSize(1);
oled->setTextColor(SSD1306_WHITE);
oled->print(F("Hello World"));
oled->show();
```

### LED
```cpp
#define LED_PIN D7

LED* led = factory.createLed(D7);

led->on();
led->off();

led->startBlink(500); //blinking time in ms
led->stopBlink();

led->setBrightness(255);
```

### Seven-Segment Display Using the MAX7219 Driver
```cpp
#define MAX7219_DATA_PIN D1
#define MAX7219_CLK_PIN  D2
#define MAX7219_CS_PIN   D0
#define MAX7219_NUM_DEVICES 3

Segment* segment = factory.createSegment(MAX7219_DATA_PIN, MAX7219_CLK_PIN, MAX7219_CS_PIN, MAX7219_NUM_DEVICES);

segment->setBrightness(50);
segment->clearAll();

segment->setBrightness(int intensity);
segment->clearAll();
segment->setChar(int overallDigitIndex, char character, bool decimalPoint);
segment->printString(const char* text);

segment->printNumber(long number);

// Display an integer (0-9999) on a specific 4-digit logical display.
// logicalDisplayNum: 0 to (totalNumDevices * 2) - 1
// Each MAX7219 hosts two 4-digit logical displays.
segment->displayPower(int logicalDisplayNum, int value);

// Display a float on a specific 4-digit logical display.
// If value is 0.0 to 999.9, displays as XXX.X (e.g., "123.4", " 23.4", "  3.4").
// If value is 1000.0 to 9999.0, displays as an integer YYYY.
// Other values (negative, too large) will display "----".
segment->displayPower(int logicalDisplayNum, float value);
```

### RGBLed
```cpp
#define RGBLED_PIN D7

RGBLED* rgb = factory.createRGBLED(RGBLED_PIN);

rgb->setColor(255, 0, 0);
rgb->setBrightness(50);
rgb->show();
```

### Rotary Encoder
```cpp
#define ENCODER_PIN_A  D0 
#define ENCODER_PIN_B  D5 
#define ENCODER_PIN_SW D7

Encoder* encoder = factory.createEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_SW, 0, 255, 1);

int16_t val = encoder->getValue();

encoder->setValue(int set_val);

//button press
if (encoder->isButtonPressed()) {

}
```

### Motor
```cpp
#define MOTOR_A_IA_PIN D5;
#define MOTOR_A_IB_PIN D6;

Motor* motor = factory.createMotor(MOTOR_A_IA_PIN, MOTOR_A_IB_PIN);

motor->forward(int speed);

motor->backward(int speed);

motor->stop();
```

The library also supports a `ShiftRegisterDevice`, which is a part of a `ShiftRegisterChain`. These devices function by leveraging the 74HC595 shift registers to control multiple outputs with fewer pins.

Currently implemented devices are the 7-Segment Display (common anode) and a LED Bargraph (or a strip of LEDs). The wiring diagram of the 7-Segment Display is as follows:


## Contributing

Contributions are welcome! Please open an issue or submit a pull request if you have any improvements or bug fixes.

## License

This project is licensed under the GPLv2 License - see the [LICENSE](LICENSE) file for details.