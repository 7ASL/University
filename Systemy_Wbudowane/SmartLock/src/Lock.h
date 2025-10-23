#include "RFIDScanner.h"

class Lock {
    private:
        RFIDScanner scanner;

    public:
        Lock(RFIDScanner scanner) : scanner(scanner) {};

};