#include "AccessVerifier.h"

void AccessVerifier::begin() {
    // Wczytuje listę autoryzowanych kart z EEPROM do pamięci RAM przy starcie urządzenia.
}

String AccessVerifier::bytesToHexString(uint8_t* uid, uint8_t length) {
    String hexString = "";
    for (uint8_t i = 0; i < length; i++) {
        if (uid[i] < 0x10) {
            hexString += "0";
        }
        hexString += String(uid[i], HEX);
    }
    return hexString;
}

String AccessVerifier::checkTagStatus(String uidHex) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected");
        return "wifi_disconnected";
    }

    HTTPClient http;
    String url = String(SERVER_URL) + "check-tag/" + uidHex;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode <= 0) {
        Serial.println("HTTP request failed: " + http.errorToString(httpCode));
        http.end();
        return "http_error";
    }

    String payload = http.getString();
    http.end();

    Serial.println("RAW response: " + payload);
  
    // Parse JSON response
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
    Serial.println("JSON parse error");
    return "json_error";
}

bool exists = doc["exists"] | false;
    if (!exists) {
        return "not_found";
    }

    const char* role = doc["role"];

    if (exists) {
        Serial.print("Tag found with role: ");
        Serial.println(role);
        if (strcmp(role, "master") == 0) {
            Serial.println("Access granted: master");
            return "master";
        } else if (strcmp(role, "user") == 0) {
            Serial.println("Access granted: user");
            return "user";
        } else {
            Serial.println("Access denied: unknown role");
        return "denied";
    }
    } else {
        Serial.println("Access denied: tag not found");
        return "not_found";
  }
}

String AccessVerifier::checkStatus(uint8_t* uid, uint8_t length) {
    String uidHex = bytesToHexString(uid, length);
    String status = checkTagStatus(uidHex);
    return status;
}

// bool AccessVerifier::isAuthorized(uint8_t* uid, uint8_t length) {
//     // Tymczasowa implementacja: porównuje UID z jedną autoryzowaną wartością.
//     const uint8_t authorizedUid[] = { 0x9D, 0xE1, 0x73, 0x6 };
//     const uint8_t authorizedLength = sizeof(authorizedUid) / sizeof(authorizedUid[0]);

//     if (length != authorizedLength) {
//         return false;
//     }

//     for (uint8_t i = 0; i < length; i++) {
//         if (uid[i] != authorizedUid[i]) {
//             return false;
//         }
//     }

//     return true;
// }
