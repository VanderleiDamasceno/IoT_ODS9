#ifndef TIMEDLOOP
#define TIMEDLOOP

#include <Arduino.h>

class timedLoop
{
private:
    unsigned long lastRun;
    unsigned int interval;
    
public:
    timedLoop(unsigned int interval);
    bool check();
    unsigned long getLastRunTime();
};


#endif