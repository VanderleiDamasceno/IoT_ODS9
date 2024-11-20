#include "mqttControl.h"

const char* topic_teste = "graph/data";

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.print("Message received on topic ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);
}

mqttControl::mqttControl(int n):
mqttClient_(espClient_),
sendMsgLoop_(1000),
connectMqttLoop_(10000)
{   
}

void mqttControl::begin()
{
    p_.begin("mqttctrl");
    server_ = p_.getString("server", "");
    port_ = p_.getInt("port", 1883);
    mqttClient_.setCallback(mqttCallback);
}

#include "globalInstances.h"

void mqttControl::connect()
{
    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Can't connect to mqtt if wifi is not connected");
        return;
    }

    if(server_ == "")
    {
        Serial.println("Mqtt broker address wan't set");
        return;
    }

    mqttClient_.setServer(server_.c_str(), port_);
    
    Serial.print("Attempting MQTT connection...");
    if (!mqttClient_.connect("ESP32Client"))
    {
        Serial.print("failed, rc=");
        Serial.print(mqttClient_.state());
        return;
    }

    Serial.println("connected");
    mqttClient_.subscribe(topic_teste);
}

void mqttControl::setServer(String server)
{
    server_ = server;
    p_.putString("server", server_);
}
void mqttControl::setPort(int port)
{
    port_ = port;
    p_.putInt("port", port_);
}
void mqttControl::printStatus()
{
    Serial.print("Server: ");
    Serial.println(server_);
    Serial.print("Port: ");
    Serial.println(port_);
    Serial.print("Status: ");
    if(mqttClient_.connected()) Serial.println("connected");
    else Serial.println("disconnected");
}

bool mqttControl::connected()
{
    return mqttClient_.connected();
}

void mqttControl::handle()
{
    if(mqttClient_.connected() && sendMsgLoop_.check())
    {
        String msg = timeCtrl.getTimeString();
        msg += ",";
        msg += irrigationCtrl.sensor_.read();
        msg += ",";
        msg += irrigationCtrl.watering_.isWatering();
        mqttClient_.publish(topic_teste, msg.c_str());
    }

    if(WiFi.status() == WL_CONNECTED && !mqttClient_.connected() && connectMqttLoop_.check())
    {
        connect();
    }
}
