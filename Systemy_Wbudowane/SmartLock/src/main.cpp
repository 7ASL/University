#include <Arduino.h>
#include <Adafruit_PN532.h>

#include "RFIDScanner.h"
#include "Lock.h"

#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
RFIDScanner scanner(nfc);
Lock lock(scanner);

void setup() {
Serial.begin(115200);
nfc.begin();
}

void loop() {
}
