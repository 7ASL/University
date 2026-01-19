#include "AccessVerifier.h"
#include <WiFiClientSecure.h>

void AccessVerifier::begin() {
    Serial.println("Loading permitted tags on startup...");
    logCooldown.start();
    fetchPermittedTags();
    refreshCooldown.start();
    Serial.println("Loaded " + String(tagCount) + " tags. Next refresh in 5 min.");
}

void AccessVerifier::update() {
    if (hasPendingLog) {
        sendLogToServer();
    }
    
    if (refreshCooldown.expired()) {
        Serial.println("Refreshing tags cache...");
        if (fetchPermittedTags()) {
            Serial.println("Refreshed " + String(tagCount) + " tags.");
        } else {
            Serial.println("Refresh failed, keeping old cache.");
        }
        refreshCooldown.start();
    }
}

String AccessVerifier::bytesToHexString(uint8_t* uid, uint8_t length) {
    String hexString = "";
    for (uint8_t i = 0; i < length; i++) {
        if (uid[i] < 0x10) hexString += "0";
        hexString += String(uid[i], HEX);
    }
    return hexString;
}

bool AccessVerifier::fetchPermittedTags() {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    String url = String(SERVER_URL) + "tags";
    http.begin(url);
    http.setTimeout(5000);  // ← 5 second timeout
    int httpCode = http.GET();
    if (httpCode != 200) {
        Serial.println("HTTP error: " + String(httpCode));
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    JsonDocument doc;  // ← Zmienione z StaticJsonDocument<4096>
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.println("JSON parse error: " + String(error.c_str()));
        return false;
    }

    tagCount = 0;
    JsonArray jsonTags = doc.as<JsonArray>();
    for (JsonObject tag : jsonTags) {
        if (tagCount >= 100) break;
        tags[tagCount].rfid_hex = tag["rfid_tag"].as<String>();
        tags[tagCount].role = tag["role"].as<String>();
        tagCount++;
    }
    return true;
}

String AccessVerifier::findTagRole(String uidHex) {
    for (uint8_t i = 0; i < tagCount; i++) {
        if (tags[i].rfid_hex.equals(uidHex)) {
            return tags[i].role;
        }
    }
    return "not_found";
}

bool AccessVerifier::logScanToServer(String uidHex, bool accessGranted, String notes) {
    // Queue the log for later sending (non-blocking)
    pendingLogHex = uidHex;
    pendingLogAccessGranted = accessGranted;
    pendingLogNotes = notes;
    hasPendingLog = true;
    Serial.println("[AccessVerifier] Log queued for sending: " + uidHex);
    return true;
}

bool AccessVerifier::sendLogToServer() {
    if (!hasPendingLog) return false;
    
    // Don't try to send again until cooldown expires
    if (logCooldown.isCooling()) {
        return false;
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[AccessVerifier] WiFi not connected, keeping log in queue");
        return false;
    }

    Serial.println("[AccessVerifier] Attempting to send log...");
    
    {
        WiFiClientSecure client;
        client.setInsecure();
        client.setTimeout(3000);
        
        HTTPClient http;
        String url = String(SERVER_URL) + "log-scan";
        http.setTimeout(3000);
        
        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");

        JsonDocument doc;
        doc["rfid_tag"] = pendingLogHex;
        doc["access_granted"] = pendingLogAccessGranted;
        if (pendingLogNotes != "") {
            doc["notes"] = pendingLogNotes;
        }

        String jsonPayload;
        serializeJson(doc, jsonPayload);
        
        Serial.println("[AccessVerifier] Sending: " + jsonPayload);

        int httpCode = http.POST(jsonPayload);
        
        http.end();
        client.stop();
        
        if (httpCode == 201 || httpCode == 200) {
            Serial.println("[AccessVerifier] Log sent! Code: " + String(httpCode));
            hasPendingLog = false;
            logCooldown.start();
            return true;
        } else {
            Serial.println("[AccessVerifier] Log failed, code: " + String(httpCode));
            logCooldown.start();
            return false;
        }
    }
}

String AccessVerifier::checkStatus(uint8_t* uid, uint8_t length) {
    String uidHex = bytesToHexString(uid, length);
    String status = findTagRole(uidHex);
    
    bool authorized = (status == "master" || status == "user");
    // logScanToServer(uidHex, authorized, "");
    
    return status;
}

bool AccessVerifier::isAuthorized(uint8_t* uid, uint8_t length) {
    String status = checkStatus(uid, length);
    return (status == "master" || status == "user");
}

void AccessVerifier::setRefreshDuration(unsigned long ms) {
    refreshCooldown.setDuration(ms);
}
