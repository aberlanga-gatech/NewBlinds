#ifndef HOMEKIT_CONFIG_H
#define HOMEKIT_CONFIG_H

#include <Arduino.h>
#include <HomeSpan.h>
#include "BlindsConfig.h"

// HomeKit Configuration
#define ACCESSORY_NAME "NewBlinds"
#define MANUFACTURER "Alfie"
#define SERIAL_NUMBER "PROTO-01"
#define MODEL_NAME "NewBlinds_P1"
#define FIRMWARE_REVISION "1.0.0"
#define SETUP_CODE "323-71-678"
#define SETUP_ID "AEBA"

// Create the Accessory and Services
void configureHomeKit()
{
    homeSpan.begin(Category::WindowCoverings, ACCESSORY_NAME);

    // WiFi setup
    homeSpan.setWifiCredentials(WIFI_SSID, WIFI_PASSWORD);

    // Accessory Information
    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();

    // Add specific characteristics for your device
    new Characteristic::Manufacturer(MANUFACTURER);
    new Characteristic::SerialNumber(SERIAL_NUMBER);
    new Characteristic::Model(MODEL_NAME);
    new Characteristic::FirmwareRevision(FIRMWARE_REVISION);
}

#endif // HOMEKIT_CONFIG_H
