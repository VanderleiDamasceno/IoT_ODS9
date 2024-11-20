#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <vector>
#include <Preferences.h>

#include "config.h"
#include "timedLoop.h"

class wifiManager
{
private:
    Preferences p;
    void _connect(String ssid, String passwd);
    void _autoConnect();
    void handleWifiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
    void _chooseNetworkFromScanAndConnect();

    bool autoConnection;
    bool waitingScanningToAutoConnect;
    bool waitingForConnection;

    timedLoop wifiAutoConnectionLoop;

public:
    std::vector<String> wifiList;
    std::vector<String> passwdList;
    wifiManager(int n, unsigned long autoConnectionCheckPeriod = 5000);
    void begin(bool autoConnection = false);

    void enableAutoConnection();
    void disableAutoConnection();

    void handle();

    void autoConnect();
    bool connect(int networkIndex);
    bool connect(String ssid);
    void addAndConnect(String ssid, String passwd);
    void disconnect();

    void addNetworkToList(String ssid, String passwd);
    bool removeNetwork(String ssid);
    bool removeNetwork(int id);

    void loadList();
    void saveList();

    void startMDNS();
    void listAvailableNetworks();

    void listSavedNetworks();
    int getNumberOfSavedNetworks();
    String getSavedNetwork(int networkIndex);
};


#endif