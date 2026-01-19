#pragma once

#include <Arduino.h>

#include "Cooldown.h"

// Manages electronic lock state with timed open/close and cooldown periods
class LockManager {
    private:
        unsigned long lockPin;
        Cooldown openTimer;
        Cooldown cooldown;

        bool isOpen;

        void unlock();
        void lock();

    public:
        LockManager(unsigned long pin, unsigned long openDuration, unsigned long cooldownDuration);
        void setup();
        void open();
        void update();
        bool onCooldown();

};