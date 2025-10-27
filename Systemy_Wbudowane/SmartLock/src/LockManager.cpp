#include "LockManager.h"

LockManager::LockManager(unsigned long pin, unsigned long openDuration, unsigned long cooldownDuration) :
            lockPin(pin),
            openTimer(openDuration),
            cooldown(cooldownDuration),
            isOpen(false)
            {}
            
void LockManager::setup() {
    pinMode(lockPin, OUTPUT);
    lock();
}

void LockManager::unlock() {
    digitalWrite(lockPin, HIGH);
    isOpen = true;
    openTimer.start();
}

void LockManager::lock() {
    digitalWrite(lockPin, LOW);
    isOpen = false;
}

void LockManager::open() {
unsigned long now = millis();

    if (!isOpen && !cooldown.isCooling()) {
        unlock();
    }
}

void LockManager::update() {
    if (isOpen && openTimer.expired()) {
        lock();
        openTimer.stop();
        cooldown.start();
    } else if (!isOpen && cooldown.expired()) {
        cooldown.stop();
    }
}