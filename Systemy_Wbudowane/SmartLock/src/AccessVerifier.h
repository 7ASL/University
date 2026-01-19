#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "credentials.h"
#include "Cooldown.h"

struct PermittedTag {
    String rfid_hex;
    String role;
};

// Verifies RFID tags against authorized list and logs scan events to server
class AccessVerifier {
private:
    PermittedTag tags[10];
    uint8_t tagCount = 0;
    Cooldown refreshCooldown;
    Cooldown logCooldown;
    
    String pendingLogHex = "";
    bool pendingLogAccessGranted = false;
    String pendingLogNotes = "";
    bool hasPendingLog = false;

    bool fetchPermittedTags();
    String findTagRole(String uidHex);
    bool sendLogToServer();

public:
    AccessVerifier() : refreshCooldown(5 * 60 * 1000UL), logCooldown(2000) {}
    String bytesToHexString(uint8_t* uid, uint8_t length);
    void begin();
    void update();
    String checkStatus(uint8_t* uid, uint8_t length);
    bool isAuthorized(uint8_t* uid, uint8_t length);
    bool logScanToServer(String uidHex, bool accessGranted, String notes = "");
    void setRefreshDuration(unsigned long ms);
};
