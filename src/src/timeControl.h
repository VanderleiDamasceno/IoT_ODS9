#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

class timeControl {
public:
    timeControl(const char* ntpServer, long gmtOffsetSec, int daylightOffsetSec);
    void begin();
    void handle();
    String getTimeString() const;

private:
    const char* ntpServer_;
    long gmtOffsetSec_;
    int daylightOffsetSec_;
    bool timeInitialized;
};

#endif