#include <Arduino.h>
#include <WiFi.h>

#include "AccessControlSystem.h"
#include "credentials.h"

AccessControlSystem accessControlSystem;

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());
    
    accessControlSystem.setup();
    }

void loop() {
    accessControlSystem.loop();
}
