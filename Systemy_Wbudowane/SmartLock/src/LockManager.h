#pragma once

#include <Arduino.h>

class LockManager {
    private:
        unsigned long lockPin;
        unsigned long openDuration;
        unsigned long cooldownDuration;

        bool isOpen;
        bool onCooldown;
        unsigned long unlockStart;
        unsigned long cooldownStart;

        void unlock();
        void lock();
        void startCooldown();

    public:
        LockManager(unsigned long pin, unsigned long openDuration, unsigned long cooldownDuration);
        void setup();
        void open();
        void update();

};