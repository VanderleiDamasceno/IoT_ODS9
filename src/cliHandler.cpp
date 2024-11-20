#include "cliHandler.h"

#include "globalInstances.h"

// Preferences prefs;
cliHandler::cliHandler(HardwareSerial& serial, int baudrate)
{
    this->serial = &serial;
    this->baudrate = baudrate;

    // prefs.begin("my-app", false);
}

void cliHandler::begin()
{
    this->serial->begin(this->baudrate);
    this->input = "";

    
    cli.setOnError(errorCallback);

    
    Command wifiCmd;
    wifiCmd = cli.addCommand("wifi", wifiCallback);
    wifiCmd.addFlagArgument("con");
    wifiCmd.addFlagArgument("dis");
    wifiCmd.addFlagArgument("add");
    wifiCmd.addFlagArgument("rem");
    wifiCmd.addFlagArgument("list");
    wifiCmd.addFlagArgument("list2");
    wifiCmd.addFlagArgument("status");
    wifiCmd.addPositionalArgument("arg1", "");
    wifiCmd.addPositionalArgument("arg2", "");

    Command restartCmd;
    restartCmd = cli.addCommand("restart", restartCallback);

    Command mqttCmd;
    mqttCmd = cli.addCommand("mqtt", mqttCallback);
    mqttCmd.addFlagArgument("con");
    mqttCmd.addFlagArgument("status");
    mqttCmd.addFlagArgument("config");
    mqttCmd.addPositionalArgument("arg1", "");
    mqttCmd.addPositionalArgument("arg2", "");

    
    Command configCmd;
    configCmd = cli.addCommand("config", configCallback);
    configCmd.addFlagArgument("hum");
    configCmd.addPositionalArgument("arg1", "");
    configCmd.addPositionalArgument("arg2", "");
}

void cliHandler::handle()
{
    while (this->serial->available())
    {
        char c = this->serial->read();
        this->serial->print(c);
        if(c != '\n') this->input += c;
        else
        {
            cli.parse(this->input);
            this->input = "";
        }
    }
}

void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

void wifiCallback(cmd* c) {
    Command cmd(c);
    Argument arg1 = cmd.getArgument("arg1");
    Argument arg2 = cmd.getArgument("arg2");
    Argument con = cmd.getArgument("con");
    Argument dis = cmd.getArgument("dis");
    Argument add = cmd.getArgument("add");
    Argument rem = cmd.getArgument("rem");
    Argument list = cmd.getArgument("list");
    Argument list2 = cmd.getArgument("list2");
    Argument status = cmd.getArgument("status");

    if(con.isSet())
    {
        if(arg2.getValue() != "")
        {
            wifiMan.addAndConnect(arg1.getValue(), arg2.getValue());
        }
        else
        {
            if(arg1.getValue() != "")
            {
                if(!wifiMan.connect(arg1.getValue()))
                    Serial.println("This wifi doesnt exist");
            }
            else wifiMan.autoConnect();
        }
    }
    else if(dis.isSet())
    {
        wifiMan.disconnect();
    }
    else if(rem.isSet())
    {
        if(wifiMan.removeNetwork(arg1.getValue())) 
            Serial.println("Removed");
        else Serial.println("This network isnt known");
    }
    else if(add.isSet())
    {
        if(arg1.getValue() != "")
        {
            wifiMan.addNetworkToList(arg1.getValue(), arg2.getValue());
        }
    }
    else if(list.isSet())
    {
        wifiMan.listSavedNetworks();
    }
    else if(list2.isSet())
    {
        wifiMan.listAvailableNetworks();
    }
    else if(status.isSet())
    {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Connected to network: " + WiFi.SSID());

            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());

            Serial.print("Signal strength (RSSI): ");
            Serial.println(WiFi.RSSI());
        } else {
            Serial.println("WiFi is not connected");
        }
    }
    else
    {
        Serial.println("Usage:");
        Serial.println(cmd.toString());
    }
}

void restartCallback(cmd* c)
{
    ESP.restart();
}


void cliHandler::parseCliCommand(String& cmd)
{
    cli.parse(cmd);
}



void mqttCallback(cmd* c) {
    Command cmd(c);
    Argument arg1 = cmd.getArgument("arg1");
    Argument arg2 = cmd.getArgument("arg2");
    Argument con = cmd.getArgument("con");
    Argument status = cmd.getArgument("status");
    Argument config = cmd.getArgument("config");
    
    if(con.isSet())
    {
        mqttCtrl.connect();
    }
    else if(status.isSet())
    {
        mqttCtrl.printStatus();
    }
    else if(config.isSet())
    {
        String server = arg1.getValue();

        if(server == "")
        {
            Serial.println("Give a server address");
            return;
        }

        mqttCtrl.setServer(server);

        String port = arg2.getValue();
        if(port != "")
        {
            mqttCtrl.setPort(port.toInt());
        }
    }
    else
    {
        Serial.println("Usage:");
        Serial.println(cmd.toString());
    }
}



void configCallback(cmd* c) {
    Command cmd(c);
    Argument arg1 = cmd.getArgument("arg1");
    Argument arg2 = cmd.getArgument("arg2");
    Argument hum  = cmd.getArgument("hum");
    
    if(hum.isSet())
    {
        if(arg1.getValue() != "")
        {
            irrigationCtrl.setHumidityThreshold(arg1.getValue().toInt());
        }
        else
        {
            Serial.print("Humidity threshold: ");
            Serial.println(irrigationCtrl.getHumidityThreshold());
        }
    }
    else
    {
        Serial.println("Usage:");
        Serial.println(cmd.toString());
    }
}