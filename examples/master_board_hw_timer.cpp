#include <Arduino.h>
#include "PeripheralFactory.h"
#include "GameManager.h"

// Hardware timer for display refresh
hw_timer_t *displayTimer = nullptr;

// Peripherals (same as original)
PeripheralFactory factory;
ShiftRegisterChain *shiftChain = nullptr;
SegmentDisplay *display1 = nullptr;
Bargraph *bargraph1 = nullptr;

void IRAM_ATTR onDisplayTimer() {
    GameManager::updateDisplays();
    factory.update();
}

void initPeripherals() {
    shiftChain = factory.createShiftRegisterChain(16, 17, 18);
    display1 = factory.createSegmentDisplay(shiftChain, 4);
    bargraph1 = factory.createBargraph(shiftChain, 10);
    display1->displayNumber(8878.0, 1);
    bargraph1->setValue(10);
    factory.update();
}

void initDisplayTimer() {
    displayTimer = timerBegin(0, 80, true);       // 1 MHz
    timerAttachInterrupt(displayTimer, &onDisplayTimer, true);
    timerAlarmWrite(displayTimer, 1000, true);    // 1 kHz
    timerAlarmEnable(displayTimer);
}

void setup() {
    Serial.begin(115200);
    initPeripherals();
    initDisplayTimer();
}

void loop() {
    GameManager::getInstance().update();
    delay(1);
}

