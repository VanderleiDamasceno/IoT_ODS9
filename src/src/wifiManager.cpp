#include "wifiManager.h"
#include "getEventType.h"

wifiManager::wifiManager(int n, unsigned long autoConnectionCheckPeriod):
wifiAutoConnectionLoop(autoConnectionCheckPeriod)
{

}

void wifiManager::begin(bool enableAutoConnection)
{
    Serial.println("begining");
    this->p.begin("wifi-config", false);

    this->loadList();

    WiFi.mode(WIFI_STA);

    WiFi.onEvent([this](arduino_event_id_t event, arduino_event_info_t info) { 
        this->handleWifiEvent(event, info); 
    });

    this->waitingScanningToAutoConnect = false;
    this->waitingForConnection = false;

    if(enableAutoConnection) this->enableAutoConnection();
    else this->disableAutoConnection();
}

void wifiManager::handle()
{
    if(this->autoConnection && this->wifiAutoConnectionLoop.check())
    {
        if(!waitingScanningToAutoConnect && !waitingForConnection)
        {
            if(WiFi.status() != WL_CONNECTED)
            {
                this->_autoConnect();
            }
        }
    }
}

void wifiManager::handleWifiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("event trigered: ");
    Serial.println(getEventName(event));

    switch (event)
    {
    case SYSTEM_EVENT_SCAN_DONE:
        if(this->waitingScanningToAutoConnect)
        {
            this->_chooseNetworkFromScanAndConnect();
            this->waitingScanningToAutoConnect = false;
        }
        WiFi.scanDelete();
        break;    
    case SYSTEM_EVENT_STA_CONNECTED:
        if(this->waitingForConnection)
        {
            Serial.println("Connected successfully");
            this->waitingForConnection = false;
        }

        break;    
    case SYSTEM_EVENT_STA_DISCONNECTED:
        if(this->waitingForConnection)
        {
            Serial.println("Could not connect to selected network");
            this->waitingForConnection = false;
        }
        break;
    default:
        break;
    }
}

void wifiManager::enableAutoConnection()
{
    Serial.println("enabling auto connect");
    this->autoConnection = true;
    this->_autoConnect();
}
void wifiManager::disableAutoConnection()
{
    Serial.println("disabling auto connect");
    this->autoConnection = false;
}

void wifiManager::_autoConnect()
{
    Serial.println("beginning of auto connect");
    this->waitingScanningToAutoConnect = true;
    
    Serial.println("getting networks...");
    WiFi.scanNetworks(true);
}

void wifiManager::_connect(String ssid, String passwd)
{
    Serial.print("connecting to ");
    Serial.println(ssid);
    
    this->waitingForConnection = true;
    WiFi.begin(ssid, passwd);

    this->startMDNS();
}

void wifiManager::startMDNS()
{
    if (!MDNS.begin(nameHost)) {
        Serial.println("Erro ao configurar o mDNS");
    }
}

void wifiManager::_chooseNetworkFromScanAndConnect()
{
    Serial.println("choosing network to connect");
    int n = WiFi.scanComplete();

    if(n == -2)
    {
        Serial.println("There isn't a network scan yet");
        return;
    }

    int bestWifiId = -1;
    int bestWifiIndexInList = -1;

    for (int i = 0; i < n; i++) {
        auto it = std::find(this->wifiList.begin(), this->wifiList.end(), WiFi.SSID(i));

        if(it != this->wifiList.end())
        {
            Serial.print(">");
            if(bestWifiId == -1 || WiFi.RSSI(i) > WiFi.RSSI(bestWifiId))
            {
                bestWifiId = i;
                bestWifiIndexInList = std::distance(wifiList.begin(), it);
            }
        }
        Serial.println(WiFi.SSID(i));
    }

    if(bestWifiIndexInList != -1)
    {
        Serial.print("chosen network: ");
        Serial.println(this->wifiList[bestWifiIndexInList]);

        this->_connect(this->wifiList[bestWifiIndexInList], this->passwdList[bestWifiIndexInList]);
    }
    else Serial.println("network not found");
}

/*----------------------------- WIFI LIST HANDLING -----------------------------*/

void wifiManager::loadList()
{
    this->wifiList.clear();
    String listStr = this->p.getString("wifiList");
    int startIndex = 0;
    int endIndex = 0;
    while ((endIndex = listStr.indexOf('\n', startIndex)) != -1)
    {
        String token = listStr.substring(startIndex, endIndex);
        this->wifiList.push_back(token);
        startIndex = endIndex + 1;
    }
    
    this->passwdList.clear();
    listStr = this->p.getString("passwdList");
    startIndex = 0;
    endIndex = 0;
    while ((endIndex = listStr.indexOf('\n', startIndex)) != -1)
    {
        String token = listStr.substring(startIndex, endIndex);
        this->passwdList.push_back(token);
        startIndex = endIndex + 1;
    }

    if(this->wifiList.size() != this->passwdList.size())
    {
        Serial.println("Error in saved wifi");
        this->wifiList.clear();
        this->passwdList.clear();
    }
}
void wifiManager::saveList()
{
    String strList = "";
    for (int i = 0; i < this->wifiList.size(); i++)
    {
        strList += this->wifiList[i]+"\n";
    }
    this->p.putString("wifiList", strList);

    strList = "";
    for (int i = 0; i < this->passwdList.size(); i++)
    {
        strList += this->passwdList[i]+"\n";
    }
    this->p.putString("passwdList", strList);
}
int wifiManager::getNumberOfSavedNetworks()
{
    return this->wifiList.size();
}
String wifiManager::getSavedNetwork(int networkIndex)
{
    return this->wifiList[networkIndex];
}
void wifiManager::listSavedNetworks()
{
    Serial.println("Saved WIFIs:");
    for (int i = 0; i < this->wifiList.size(); i++)
    {
        Serial.print("    \"");
        Serial.print(this->wifiList[i]);
        Serial.print("\" \"");
        Serial.print(this->passwdList[i]);
        Serial.println("\"");
    }
}
void wifiManager::addNetworkToList(String ssid, String passwd)
{
    for (int i = 0; i < this->wifiList.size(); i++)
    {
        if(this->wifiList[i] == ssid)
        {
            this->passwdList[i] = passwd;
            return;
        }
    }

    this->wifiList.push_back(ssid);
    this->passwdList.push_back(passwd);

    this->saveList();
}
bool wifiManager::removeNetwork(String ssid)
{
    auto it = std::find(this->wifiList.begin(), this->wifiList.end(), ssid);
    
    if(it == this->wifiList.end()) return false;

    int indexToRemove = std::distance(wifiList.begin(), it);

    this->wifiList.erase(it);
    this->passwdList.erase(this->passwdList.begin() + indexToRemove);
    this->saveList();

    return true;
}
bool wifiManager::removeNetwork(int id)
{
    if(id >= this->wifiList.size()) return false;
    
    this->wifiList.erase(this->wifiList.begin() + id);
    this->passwdList.erase(this->passwdList.begin() + id);
    this->saveList();

    return true;
}


/*----------------------------- OTHER PUBLIC FUNCTIONS -----------------------------*/

void wifiManager::autoConnect()
{
    this->_autoConnect();
}

void wifiManager::addAndConnect(String ssid, String passwd)
{
    this->addNetworkToList(ssid, passwd);
    this->_connect(ssid, passwd);
}

bool wifiManager::connect(String ssid)
{
    for (int i = 0; i < this->wifiList.size(); i++)
    {
        if(this->wifiList[i] == ssid)
        {
            this->_connect(ssid, this->passwdList[i]);
            return true;
        }
    }

    return false;
}

bool wifiManager::connect(int networkIndex)
{
    if(networkIndex >= this->wifiList.size()) return false;

    this->_connect(this->wifiList[networkIndex], this->passwdList[networkIndex]);
    return true;
}

void wifiManager::disconnect()
{
    WiFi.disconnect();
}

void wifiManager::listAvailableNetworks()
{
    Serial.print("Scanning for available networks...");
    
    // Start the scan
    int n = WiFi.scanNetworks();
    
    if (n == 0)
    {
        Serial.print("No networks found.");
    }
    else
    {
        Serial.print("Networks found:");
        
        for (int i = 0; i < n; ++i)
        {
            // Print SSID, signal strength (RSSI), and encryption type
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(" dBm)");
            Serial.print(" Encryption: ");
            Serial.print(WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Encrypted");
            
            delay(10);
        }
    }

}