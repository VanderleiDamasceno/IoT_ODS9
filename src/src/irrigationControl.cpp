#include "irrigationControl.h"

irrigationControl::irrigationControl(unsigned long default_watering_time) :
watering_(default_watering_time),
sensorReadingLoop_(1000)
{
}

void irrigationControl::begin(int valvePin, int sensorPin)
{
    Serial.println("starting irrigationCtrl");
    p_.begin("irrCtrl");
    minHumidityThreshold_ = p_.getInt("minHum", 0);
    watering_.begin(valvePin);
    sensor_.begin(sensorPin);
}

void irrigationControl::handle()
{
    watering_.handle();

    if(sensorReadingLoop_.check() && !watering_.isWatering())
    {
        int sensorValue = sensor_.read();

        Serial.print("hum: ");
        Serial.println(sensorValue);

        if(sensorValue > minHumidityThreshold_)
        {
            watering_.start();
        }
    }
}

void irrigationControl::setHumidityThreshold(int threshold)
{
    minHumidityThreshold_ = threshold;
    p_.putInt("minHum", minHumidityThreshold_);
    Serial.print("humd set to ");
    Serial.println(p_.getInt("minHum", -1));
}

int irrigationControl::getHumidityThreshold()
{
    return minHumidityThreshold_;
}
