#include "AccessVerifier.h"

void AccessVerifier::begin() {
    // Wczytuje listę autoryzowanych kart z EEPROM do pamięci RAM przy starcie urządzenia.
}

bool AccessVerifier::isAuthorized(uint8_t* uid, uint8_t length) {
    // Tymczasowa implementacja: porównuje UID z jedną autoryzowaną wartością.
    const uint8_t authorizedUid[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    const uint8_t authorizedLength = sizeof(authorizedUid) / sizeof(authorizedUid[0]);

    if (length != authorizedLength) {
        return false;
    }

    for (uint8_t i = 0; i < length; i++) {
        if (uid[i] != authorizedUid[i]) {
            return false;
        }
    }

    return true;
}