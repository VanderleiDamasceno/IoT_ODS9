#ifndef CLIHANDLER_H
#define CLIHANDLER_H

#include <Arduino.h>
#include <SimpleCLI.h>
#include <Preferences.h>

class cliHandler
{
private:
    int baudrate;
    HardwareSerial* serial;
    String input;

    SimpleCLI cli;

public:
    cliHandler(HardwareSerial& serial, int baudrate);
    void begin();
    void handle();
    void parseCliCommand(String& cmd);
};


void errorCallback(cmd_error* e);
void wifiCallback(cmd* c);
void restartCallback(cmd* c);
void mqttCallback(cmd* c);
void configCallback(cmd* c);


#endif