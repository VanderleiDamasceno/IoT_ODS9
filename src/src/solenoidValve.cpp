#include "solenoidValve.h"

// #include "simulateSensor.h"

solenoidValve::solenoidValve()
{
}

void solenoidValve::begin(int solenoidPin)
{
    solenoidPin_ = solenoidPin;
    pinMode(solenoidPin_, OUTPUT);
}

void solenoidValve::open()
{
    if(stt_) return;

    digitalWrite(solenoidPin_, LOW);
    // sim_sensor.valve_stt = true;
    stt_ = HIGH;
}

void solenoidValve::close()
{
    if(!stt_) return;
    
    digitalWrite(solenoidPin_, HIGH);
    // sim_sensor.valve_stt = false;
    stt_ = LOW;
}

bool solenoidValve::state()
{
    return stt_;
}