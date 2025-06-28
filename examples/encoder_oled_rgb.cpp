/*
An example code for controlling an RGB LED color with an encoder and displaying the values on an OLED screen.

Jakub Pelc, 2025

GNU General Public License v2.0
*/

#include <PeripheralFactory.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET_PIN -1
#define I2C_ADDRESS 0x3C

#define ENCODER_PIN_A  D0 
#define ENCODER_PIN_B  D5 
#define ENCODER_PIN_SW D7

PeripheralFactory factory;
OLEDDisplay* oled = factory.createOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);
LED* led = factory.createLed(D4);
Encoder* encoder = factory.createEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_SW, 0, 255, 1);
RGBLED* rgb = factory.createRGBLED(D6);

void setup() {
	Serial.begin(115200);
	Serial.println(F("Booting..."));

	factory.init();

	rgb->setColor(255, 0, 0);
	rgb->setBrightness(50);
	rgb->show();

	led->on();
}

void loop() {
	enum RGBMode { MODE_R, MODE_G, MODE_B, MODE_BRIGHTNESS };
	static RGBMode mode = MODE_R;
	static uint8_t r = 0, g = 0, b = 0, brightness = 255;
	static int16_t lastEncoderValue = -1;

	auto setEncoderRange = [&]() {
		switch (mode) {
			case MODE_R: encoder->setValue(r); break;
			case MODE_G: encoder->setValue(g); break;
			case MODE_B: encoder->setValue(b); break;
			case MODE_BRIGHTNESS: encoder->setValue(brightness); break;
		}
	};

	factory.update();

	unsigned long currentMillis = millis();

	if (encoder->isButtonPressed()) {
		mode = static_cast<RGBMode>((mode + 1) % 4);
		setEncoderRange();
		//buzzer->buzz(100);
	}

	int16_t val = encoder->getValue();
	if (val != lastEncoderValue) {
		lastEncoderValue = val;
		switch (mode) {
			case MODE_R: r = val; led->setBrightness(r); break;
			case MODE_G: g = val; break;
			case MODE_B: b = val; break;
			case MODE_BRIGHTNESS: brightness = val; break;
		}
	}

	if (currentMillis % 50 == 0) {
		oled->clear();
		oled->setCursor(0, 0);
		oled->setTextSize(1);
		oled->setTextColor(SSD1306_WHITE);
		oled->print(F("Mode: "));
		switch (mode) {
			case MODE_R: oled->println(F("R")); break;
			case MODE_G: oled->println(F("G")); break;
			case MODE_B: oled->println(F("B")); break;
			case MODE_BRIGHTNESS: oled->println(F("Brightness")); break;
		}
		oled->print(F("R: ")); oled->println(r);
		oled->print(F("G: ")); oled->println(g);
		oled->print(F("B: ")); oled->println(b);
		oled->print(F("Br: ")); oled->println(brightness);
		oled->show();
	}
	
	rgb->setColor(r, g, b);
	rgb->setBrightness(brightness);
	rgb->show();
}