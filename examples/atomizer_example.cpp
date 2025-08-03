#include <Arduino.h>
#include "PeripheralFactory.h"

// Create a peripheral factory instance
PeripheralFactory factory;

// Create atomizer peripheral (connected to pin D2)
Atomizer* atomizer;

void setup() {
    Serial.begin(115200);
    Serial.println("Atomizer Example Starting...");
    
    // Create atomizer on pin D2
    atomizer = factory.createAtomizer(D2);
    
    Serial.println("Atomizer created and ready!");
}

void loop() {
    // Update all peripherals (this handles the non-blocking timing)
    factory.update();
    
    // Example: Trigger atomizer every 5 seconds
    static unsigned long lastTrigger = 0;
    if (millis() - lastTrigger >= 5000) {
        if (!atomizer->isActive()) {
            Serial.println("Triggering atomizer...");
            atomizer->toggle();
            lastTrigger = millis();
        }
    }
    
    // You can trigger the atomizer from other events too:
    // - Button press
    // - Sensor reading
    // - Timer callback
    // - etc.
}
