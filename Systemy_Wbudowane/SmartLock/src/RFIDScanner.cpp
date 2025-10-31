#include "RFIDScanner.h"

void RFIDScanner::begin() {
    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
    Serial.print("Didn't find PN53x board");
            // while (1); // Stop program execution
    }
    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    nfc.SAMConfig();

    nfc.setPassiveActivationRetries(1);
}

bool RFIDScanner::scan() {
    if(cooldown.isCooling()) {
        return false;
    }

    bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidBuffer, &uidLength);

    if (success && uidLength > 0) {
        cooldown.start();
        return true;
    }

    uidLength = 0;
    return false;
}

uint8_t* RFIDScanner::getUidBytes(uint8_t &length) {
    if(uidLength == 0) {
        length = 0;
        return nullptr;
    } else {
        length = uidLength;
        return uidBuffer;
    }
}