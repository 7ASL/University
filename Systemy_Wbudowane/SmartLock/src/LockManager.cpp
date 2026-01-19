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
    Serial.println("[LockManager] unlock -> opened, openTimer started");

}

void LockManager::lock() {
    digitalWrite(lockPin, LOW);
    isOpen = false;
    Serial.println("[LockManager] lock -> closed, openTimer stopped");

}

void LockManager::open() {
    if (!isOpen && !cooldown.isCooling()) {
        Serial.println("[LockManager] open() -> allowed, calling unlock()");
        unlock();
    } else {
        Serial.print("[LockManager] open() -> denied isOpen=");
        Serial.print(isOpen);
        Serial.print(" cooldown=");
        Serial.println(cooldown.isCooling());
    }
}

void LockManager::update() {
    if (openTimer.isRunning() && openTimer.expired()) {
        Serial.println("[LockManager] update: openTimer expired -> locking and starting cooldown");
        lock();
        openTimer.stop();
        cooldown.start();
    }
    
    if (cooldown.isRunning() && cooldown.expired()) {
        Serial.println("[LockManager] update: cooldown expired -> stopping cooldown");
        cooldown.stop();
    }
}

bool LockManager::onCooldown() {
    return (openTimer.isCooling() || cooldown.isCooling());
}