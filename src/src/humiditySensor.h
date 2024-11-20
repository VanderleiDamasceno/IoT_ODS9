#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include <Arduino.h>

class humiditySensor
{
private:
    int sensorPin_;
public:
    humiditySensor();
    void begin(int sensorPin);
    int read();
};


#endif