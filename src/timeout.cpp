#include "timeout.h"

timeout::timeout()
{
    timeout(0);
}

timeout::timeout(unsigned long time)
{   
    time_ = time;
    isRunning_ = false;
}   

bool timeout::isRunning()
{
    return isRunning_;
}

void timeout::setTime(unsigned long time)
{
    time_ = time;
}

void timeout::start()
{
    startTime_ = millis();
    isRunning_ = true;
}
void timeout::start(unsigned long time)
{
    time_ = time;
    start();
}

void timeout::stop()
{
    isRunning_ = false;
}

bool timeout::finished()
{
    if(!isRunning_) return false;

    if(millis() - startTime_ > time_)
    {
        isRunning_ = false;
        return true;
    }

    return false;
}