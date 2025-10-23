#include <Adafruit_PN532.h>


class RFIDScanner {
    private:
        Adafruit_PN532 nfc;

    public:
        RFIDScanner(Adafruit_PN532 nfc) : nfc(nfc) {}
        void scan();
};