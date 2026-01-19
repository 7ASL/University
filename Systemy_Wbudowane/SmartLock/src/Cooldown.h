#pragma once

#include <Arduino.h>

// Timer utility for managing cooldown and duration-based operations
class Cooldown {
    private:
        unsigned long duration_ms;
        unsigned long start_ms;
        bool running;

    public:
        Cooldown(unsigned long duration = 0UL) : duration_ms(duration), start_ms(0), running(false) {}
        void start() { start_ms = millis(); running = true; }
        void reset() { start(); }
        void stop() { running = false; }
        void setDuration(unsigned long d) { duration_ms = d; }

        bool isCooling() const {
            if (!running) return false;
            return (millis() - start_ms) < duration_ms;
        }

        bool expired() const {
            if (!running) return false;
            return (millis() - start_ms) >= duration_ms;
        }

        bool isRunning() const { return running; }

        unsigned long remaining() const {
            if (!running) return 0;
            unsigned long elapsed = millis() - start_ms;
            return (elapsed >= duration_ms) ? 0 : (duration_ms - elapsed);
        }
};