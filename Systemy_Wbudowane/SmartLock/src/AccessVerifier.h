#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "credentials.h"

class AccessVerifier {
    private:
        String bytesToHexString(uint8_t* uid, uint8_t length);
        String checkTagStatus(String uidHex);

    public:
        void begin();
        String checkStatus(uint8_t* uid, uint8_t length);
        bool isAuthorized(uint8_t* uid, uint8_t length);
        
        // Przyszłe funkcje do zarządzania listą autoryzowanych kart po implementacji CYD
        // void addCard();
        // void removeCard();
};  