#ifndef IRRIGATIONCONTROL_H
#define IRRIGATIONCONTROL_H

#include "config.h"
#include "watering.h"
#include "humiditySensor.h"
#include "timedLoop.h"

class irrigationControl
{
private:
    timedLoop sensorReadingLoop_;
    int minHumidityThreshold_;
    Preferences p_;

public:
    watering watering_;
    humiditySensor sensor_;
    irrigationControl(unsigned long default_watering_time);
    void begin(int valvePin, int sensorPin);
    void handle();
    void setHumidityThreshold(int threshold);
    int getHumidityThreshold();
};

#endif