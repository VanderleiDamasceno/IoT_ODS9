#include "timedLoop.h"

timedLoop::timedLoop(unsigned int interval)
{
    this->lastRun = -interval;
    this->interval = interval;
}

bool timedLoop::check()
{
    unsigned long now = millis();
    
    if(now - this->lastRun < interval)
    {
        return false;
    }
    
    this->lastRun = now;
    return true;
}
unsigned long timedLoop::getLastRunTime()
{
    return this->lastRun;
}