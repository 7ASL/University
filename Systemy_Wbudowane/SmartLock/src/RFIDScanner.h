#pragma once

#include <Adafruit_PN532.h>


class RFIDScanner {
    private:
        Adafruit_PN532 &nfc;
        
        uint8_t uidBuffer[7];
        uint8_t uidLength;

    public:
        RFIDScanner(Adafruit_PN532 &nfc) : nfc(nfc), uidLength(0) {}
        void begin();
        bool scan();
        uint8_t* getUidBytes(uint8_t &length);
};