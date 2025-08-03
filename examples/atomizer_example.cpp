#include <Arduino.h>
#include "PeripheralFactory.h"

// Create a peripheral factory instance
PeripheralFactory factory;

// Create atomizer peripheral (connected to pin D2)
Atomizer* atomizer;
Button* button;

void setup() {
    Serial.begin(115200);
    Serial.println("Atomizer Example Starting...");
    
    // Create atomizer on pin D2
    atomizer = factory.createAtomizer(D2);
    
    // Create button on pin D7 for manual triggering
    button = factory.createButton(D7);
    
    // Set up button callback - this is now safe because toggle() doesn't do pin operations
    button->setClickHandler([](Button2& btn) {
        Serial.println("Button pressed - triggering atomizer...");
        atomizer->toggle(); // Safe to call from callback!
    });
    
    Serial.println("Atomizer created and ready!");
    Serial.println("Press button on D7 to trigger atomizer, or it will auto-trigger every 5 seconds");
}

void loop() {
    // Update all peripherals (this handles the non-blocking timing and pin operations)
    factory.update();
    
    // Example: Trigger atomizer every 5 seconds automatically
    static unsigned long lastTrigger = 0;
    if (millis() - lastTrigger >= 5000) {
        if (!atomizer->isActive()) {
            Serial.println("Auto-triggering atomizer...");
            atomizer->toggle(); // Safe to call anytime!
            lastTrigger = millis();
        }
    }
    
    // All pin operations happen in update() - callbacks are safe!
}
