#pragma once

#include <Adafruit_PN532.h>

#include "RFIDScanner.h"
#include "LockManager.h"
#include "AccessVerifier.h"

#define PN532_SCK  (18)
#define PN532_MISO (19)
#define PN532_MOSI (23)
#define PN532_SS   (5)

class  AccessControlSystem {
    private:
        Adafruit_PN532 nfc;
        RFIDScanner scanner;
        LockManager lock;
        AccessVerifier verifier;
    public:
        AccessControlSystem() : 
            nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS),
            scanner(nfc, 1000),
            lock(32, 2500, 2500)
            {};

        void setup() {
            lock.setup();
            scanner.begin();
            Serial.println("[LockManager] setup -> locked");

        }

        void loop() {
            lock.update();
            if(scanner.scan() && !lock.onCooldown()) {
                uint8_t length;
                uint8_t* uid = scanner.getUidBytes(length);

                Serial.print(" UID Length: "); Serial.println(length);
                Serial.print("UID Value: ");
                for (uint8_t i = 0; i < length; i++) {
                    Serial.print(" 0x"); Serial.print(uid[i], HEX);
                }

                if (verifier.isAuthorized(uid, length)) {
                    lock.open();
                    Serial.println(" - Access granted");    
                } else {
                    Serial.println(" - Access denied");
                }

            }
        }
};