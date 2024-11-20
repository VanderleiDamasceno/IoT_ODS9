#include <Arduino.h>

#include "globalInstances.h"
#include "ota.h"
// #include "simulateSensor.h"

void setup() {
    Serial.begin(115200);
    espcli.begin();
    wifiMan.begin(true);
    irrigationCtrl.begin(VALVE_PIN, SENSOR_PIN);
    mqttCtrl.begin();
    timeCtrl.begin();

    setupOTA();
}

void loop() {
    espcli.handle();
    wifiMan.handle();
    irrigationCtrl.handle();
    // sim_sensor.handle();
    mqttCtrl.handle();
    timeCtrl.handle();

    #if defined(ESP32_RTOS) && defined(ESP32)
    #else // If you do not use FreeRTOS, you have to regulary call the handle method.
        ArduinoOTA.handle();
    #endif
}