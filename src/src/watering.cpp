#include "watering.h"

watering::watering(unsigned long defaultWateringTime)
{
    p.begin("watering");
    wateringTime_ = p.getULong("time", defaultWateringTime);
    wateringTimer_.setTime(wateringTime_);
}

void watering::begin(int valvePin)
{
    valve_.begin(valvePin);
}
void watering::handle()
{
    if(wateringTimer_.finished())
    {
        stop();
    }
}

void watering::start()
{
    Serial.println("Start watering");

    wateringTimer_.start();
    valve_.open();
}

void watering::stop()
{
    Serial.println("Stop watering");

    valve_.close();
    wateringTimer_.stop();
}

bool watering::isWatering()
{
    return wateringTimer_.isRunning();
}

unsigned long watering::getWateringTime()
{
    return wateringTime_;
}
void watering::setWateringTime(unsigned long time)
{
    wateringTime_ = time;
    wateringTimer_.setTime(wateringTime_);
    p.putULong("time", wateringTime_);
}