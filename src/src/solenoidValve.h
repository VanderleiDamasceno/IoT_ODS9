#ifndef SOLENOIDVALVE_H
#define SOLENOIDVALVE_H

#include "Arduino.h"

class solenoidValve
{
private:
    int solenoidPin_;
    bool stt_;
public:
    solenoidValve();
    void begin(int solenoidPin);
    void open();
    void close();
    bool state();
};

#endif