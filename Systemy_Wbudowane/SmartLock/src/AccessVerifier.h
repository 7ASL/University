#pragma once

#include <Arduino.h>

class AccessVerifier {
    public:
        void begin();
        bool isAuthorized(uint8_t* uid, uint8_t length);
        
        // Przyszłe funkcje do zarządzania listą autoryzowanych kart po implementacji CYD
        // void addCard();
        // void removeCard();
};  