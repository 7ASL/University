#pragma once

#include <Adafruit_PN532.h>

#include "RFIDScanner.h"
#include "LockManager.h"
#include "AccessVerifier.h"

#define PN532_SCK  (18)
#define PN532_MISO (19)
#define PN532_MOSI (23)
#define PN532_SS   (5)

// Main access control system that coordinates RFID scanning, lock control, and verification
class AccessControlSystem {
    private:
        Adafruit_PN532 nfc;
        RFIDScanner scanner;
        LockManager lock;
        AccessVerifier verifier;

        String note;
    public:
        AccessControlSystem() : 
            nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS),
            scanner(nfc, 1000),
            lock(32, 2500, 1000)
            {};

        void setup() {
            lock.setup();
            scanner.begin();
            verifier.begin();
            Serial.println("[LockManager] setup -> locked");
        }

        void loop() {
            lock.update();
            
            if (!lock.onCooldown()) {
                verifier.update();
            }
            
            if(scanner.scan() && !lock.onCooldown()) {
                uint8_t length;
                uint8_t* uid = scanner.getUidBytes(length);

                Serial.print(" UID Length: "); Serial.println(length);
                Serial.print("UID Value: ");
                for (uint8_t i = 0; i < length; i++) {
                    Serial.print(" 0x"); Serial.print(uid[i], HEX);
                }

                bool status = verifier.isAuthorized(uid, length);

                if (status) {
                    lock.open();
                    note = "Access granted";
                } else {
                    note = "Access denied";
                }
                
                // Queue log (non-blocking) - happens after lock state is set
                verifier.logScanToServer(verifier.bytesToHexString(uid, length), status, note);

            }
        }
};