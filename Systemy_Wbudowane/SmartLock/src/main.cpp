#include <Arduino.h>

#include "AccessControlSystem.h"

AccessControlSystem accessControlSystem;

void setup() {
    Serial.begin(115200);
    accessControlSystem.setup();
}

void loop() {
    accessControlSystem.loop();
}
