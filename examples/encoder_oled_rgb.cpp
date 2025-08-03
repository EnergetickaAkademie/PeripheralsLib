/*
An example code for controlling an RGB LED color with an encoder and displaying the values on an OLED screen.
Note: Encoder functionality is only available on ESP32, not ESP8266.

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
#ifndef ESP8266
Encoder* encoder = factory.createEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_SW, 0, 255, 1);
#endif
RGBLED* rgb = factory.createRGBLED(D6);

void setup() {
	Serial.begin(115200);
	Serial.println(F("Booting..."));

	rgb->setColor(255, 0, 0);
	rgb->setBrightness(50);
	rgb->show();

	led->on();

#ifdef ESP8266
	Serial.println(F("ESP8266 detected - encoder functionality disabled"));
	Serial.println(F("RGB LED will cycle through colors automatically"));
#else
	Serial.println(F("ESP32 detected - encoder functionality enabled"));
#endif
}

void loop() {
#ifndef ESP8266
	// ESP32 version with encoder control
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

	if (encoder->isButtonPressed()) {
		mode = static_cast<RGBMode>((mode + 1) % 4);
		setEncoderRange();
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

	unsigned long currentMillis = millis();
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

#else
	// ESP8266 version with automatic color cycling (no encoder)
	static uint8_t r = 255, g = 0, b = 0;
	static uint8_t brightness = 50;
	static unsigned long lastUpdate = 0;
	static int colorStep = 0;

	factory.update();

	// Update RGB colors every 100ms for smooth transitions
	if (millis() - lastUpdate >= 100) {
		lastUpdate = millis();
		
		// Simple color cycling: Red -> Green -> Blue -> Red
		switch (colorStep / 100) {
			case 0: // Fade red to green
				r = max(0, 255 - (colorStep % 100) * 3);
				g = min(255, (colorStep % 100) * 3);
				b = 0;
				break;
			case 1: // Fade green to blue  
				r = 0;
				g = max(0, 255 - (colorStep % 100) * 3);
				b = min(255, (colorStep % 100) * 3);
				break;
			case 2: // Fade blue to red
				r = min(255, (colorStep % 100) * 3);
				g = 0;
				b = max(0, 255 - (colorStep % 100) * 3);
				break;
		}
		
		colorStep++;
		if (colorStep >= 300) colorStep = 0;

		led->setBrightness(r);
	}

	// Update OLED display every 500ms
	static unsigned long lastDisplayUpdate = 0;
	if (millis() - lastDisplayUpdate >= 500) {
		lastDisplayUpdate = millis();
		
		oled->clear();
		oled->setCursor(0, 0);
		oled->setTextSize(1);
		oled->setTextColor(SSD1306_WHITE);
		oled->println(F("ESP8266 Mode"));
		oled->println(F("Auto Color Cycle"));
		oled->print(F("R: ")); oled->println(r);
		oled->print(F("G: ")); oled->println(g);
		oled->print(F("B: ")); oled->println(b);
		oled->print(F("Br: ")); oled->println(brightness);
		oled->show();
	}
	
	rgb->setColor(r, g, b);
	rgb->setBrightness(brightness);
	rgb->show();
#endif
}