#ifndef WATERING_H
#define WATERING_H

#include <Arduino.h>
#include <Preferences.h>

#include "solenoidValve.h"
#include "timeout.h"

class watering
{
private:
    solenoidValve valve_;

    timeout wateringTimer_;
    unsigned long wateringTime_;

    Preferences p;

public:
    watering(unsigned long defaultWateringTime);
    void begin(int valvePin);
    void handle();
    void start();
    void stop();
    bool isWatering();

    unsigned long getWateringTime();
    void setWateringTime(unsigned long time);
};


#endif