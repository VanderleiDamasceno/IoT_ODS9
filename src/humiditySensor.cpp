#include "humiditySensor.h"

// #include "simulateSensor.h"

humiditySensor::humiditySensor()
{
}

void humiditySensor::begin(int sensorPin)
{
    sensorPin_ = sensorPin;
    pinMode(sensorPin_, OUTPUT);
}

int humiditySensor::read()
{
    const int numSamples = 100;
    long total = 0;

    for (int i = 0; i < numSamples; ++i) {
        total += analogRead(sensorPin_);
    }

    return total/numSamples;
}
