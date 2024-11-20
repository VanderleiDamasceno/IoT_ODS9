#ifndef MQTTCONTROL_H
#define MQTTCONTROL_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>

#include "timedLoop.h"

class mqttControl
{
private:
    
    WiFiClient espClient_;
    PubSubClient mqttClient_;
    String server_;
    int port_;
    Preferences p_;
    timedLoop connectMqttLoop_;
    timedLoop sendMsgLoop_;

public:
    mqttControl(int n);
    void setServer(String server);
    void setPort(int port);
    void begin();
    void handle();
    void connect();
    void printStatus();
    bool connected();

    // void sendGraphData(int humidity, int time)
};

void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif