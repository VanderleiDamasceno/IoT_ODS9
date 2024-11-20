#include "timeControl.h"


// Constructor
timeControl::timeControl(const char* ntpServer, long gmtOffsetSec, int daylightOffsetSec)
    : ntpServer_(ntpServer), gmtOffsetSec_(gmtOffsetSec), daylightOffsetSec_(daylightOffsetSec), timeInitialized(false) {}

// Begin function to initialize NTP only if Wi-Fi is connected
void timeControl::begin() {
    // Check if Wi-Fi is connected (handled by another class)
    if (WiFi.status() == WL_CONNECTED) {
        configTime(gmtOffsetSec_, daylightOffsetSec_, ntpServer_);
        timeInitialized = true;
        Serial.println("NTP client initialized");
    }
}

// Handle function to be called in the loop to reinitialize if necessary
void timeControl::handle() {
    if (!timeInitialized && WiFi.status() == WL_CONNECTED) {
        begin();
    }
}

// Method to get the current time as a String
String timeControl::getTimeString() const {
    if(!timeInitialized) return "";
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "";
    }
    
    char buffer[25];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}