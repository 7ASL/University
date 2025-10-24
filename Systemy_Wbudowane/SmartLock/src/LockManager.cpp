#include "LockManager.h"

LockManager::LockManager(unsigned long pin, unsigned long openDuration, unsigned long cooldownDuration) :
            lockPin(pin),
            openDuration(openDuration),
            cooldownDuration(cooldownDuration),
            isOpen(false),
            onCooldown(false),
            unlockStart(0),
            cooldownStart(0)
            {};
            
void LockManager::setup() {
    pinMode(lockPin, OUTPUT);
}

void LockManager::unlock() {
    digitalWrite(lockPin, HIGH);
    isOpen = true;
    unlockStart = millis();
}

void LockManager::lock() {
    digitalWrite(lockPin, LOW);
    isOpen = false;
}

void LockManager::startCooldown() {
    onCooldown = true;
    cooldownStart = millis();
}

void LockManager::open() {
unsigned long now = millis();

    if (!isOpen && !onCooldown) {
        unlock();
    }
}

void LockManager::update() {
    unsigned long currentTime = millis();
    if (isOpen) {
        if (currentTime - unlockStart >= openDuration) {
            lock();
            startCooldown();
        }
    } else if (onCooldown) {
        if (currentTime - cooldownStart >= cooldownDuration) {
            onCooldown = false;
        }
    }
}