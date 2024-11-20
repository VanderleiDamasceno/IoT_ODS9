#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <Arduino.h>

class timeout
{
private:
    unsigned long time_;
    unsigned long startTime_;
    bool isRunning_;
public:
    timeout();
    timeout(unsigned long time);
    bool isRunning();
    void start();
    void start(unsigned long time);
    void setTime(unsigned long time);
    void stop();
    bool finished();
};

#endif