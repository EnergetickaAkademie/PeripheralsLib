#include <PeripheralFactory.h>

// Example for ESP8266 - demonstrates peripheral usage without encoder
PeripheralFactory factory;

// Create peripherals
LED* led;
RGBLED* rgb;
Button* button;

void setup() {
    Serial.begin(115200);
    Serial.println(F("ESP8266 Peripherals Example"));
    
    // Create peripherals compatible with ESP8266
    led = factory.createLed(D4);      // Built-in LED on most ESP8266 boards
    rgb = factory.createRGBLED(D6);   // RGB LED strip
    button = factory.createButton(D7); // Push button
    
    // Initialize RGB with red color
    rgb->setColor(255, 0, 0);
    rgb->setBrightness(50);
    rgb->show();
    
    led->on();
    
    Serial.println(F("Setup complete - press button to change colors"));
}

void loop() {
    // Update all peripherals
    factory.update();
    
    // Static variables for color cycling
    static uint8_t colorIndex = 0;
    static unsigned long lastColorChange = 0;
    static bool buttonPressed = false;
    
    // Check button state
    if (button->isPressed() && !buttonPressed) {
        buttonPressed = true;
        colorIndex = (colorIndex + 1) % 6;
        
        // Set different colors based on index
        switch(colorIndex) {
            case 0: rgb->setColor(255, 0, 0); Serial.println("Red"); break;
            case 1: rgb->setColor(0, 255, 0); Serial.println("Green"); break;
            case 2: rgb->setColor(0, 0, 255); Serial.println("Blue"); break;
            case 3: rgb->setColor(255, 255, 0); Serial.println("Yellow"); break;
            case 4: rgb->setColor(255, 0, 255); Serial.println("Magenta"); break;
            case 5: rgb->setColor(0, 255, 255); Serial.println("Cyan"); break;
        }
        rgb->show();
        
        // Blink LED to acknowledge button press
        led->startBlink(100);
        lastColorChange = millis();
    }
    
    if (!button->isPressed()) {
        buttonPressed = false;
    }
    
    // Stop LED blinking after 1 second
    if (millis() - lastColorChange > 1000) {
        led->stopBlink();
        led->on();
    }
}
