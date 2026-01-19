#pragma once

#include <Adafruit_PN532.h>

#include "Cooldown.h"

// RFID card scanner with cooldown to prevent duplicate reads
class RFIDScanner {
    private:
        Adafruit_PN532 &nfc;
        Cooldown cooldown;
        
        uint8_t uidBuffer[7];
        uint8_t uidLength;

    public:
        RFIDScanner(Adafruit_PN532 &nfc, unsigned long cooldownDuration) : nfc(nfc), cooldown(cooldownDuration), uidLength(0) {}
        void begin();
        bool scan();
        uint8_t* getUidBytes(uint8_t &length);
};