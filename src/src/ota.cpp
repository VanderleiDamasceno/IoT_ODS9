#include "ota.h"


#if defined(ESP32_RTOS) && defined(ESP32)
void ota_handle( void * parameter ) {
    for (;;) {
        ArduinoOTA.handle();
        delay(3500);
    }
}
#endif

void setupOTA() {
    ArduinoOTA.onStart([]() {
        //NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
        else // U_SPIFFS
        type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.print("Progress: ");
        Serial.print((progress / (total / 100)));
        Serial.println("%");
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.print("Error[");
        Serial.print(error);
        Serial.println("]: ");
        if (error == OTA_AUTH_ERROR) Serial.println("\nAuth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("\nBegin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("\nConnect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("\nReceive Failed");
        else if (error == OTA_END_ERROR) Serial.println("\nEnd Failed");
    });

    ArduinoOTA.begin();

    Serial.println("OTA Initialized");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());

    #if defined(ESP32_RTOS) && defined(ESP32)
    xTaskCreate(
        ota_handle,          /* Task function. */
        "OTA_HANDLE",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
    #endif
}