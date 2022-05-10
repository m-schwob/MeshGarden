#include <Arduino.h>
#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_ADS1X15.h>
// DO NOT REMOVE INCLUDES UNLESS UPDATING INI FILE!!!!

#if defined(ESP32)
#include "mesh_bridge.h"
#elif defined(ESP8266)
#include "mesh_node.h"
#include "soil_moisure_sensor_grove_v1.0.cpp"
MeshNode* mesh = NULL;
SoilMoisureSensorGroveV1* soil_moisture = NULL;
#endif


#define BAUD_RATE 115200


void setup(void)
{
    Serial.begin(BAUD_RATE);
    Serial.println(); Serial.println(); // make lines space in the begining
    #ifdef ESP32
    init_node();
    #else
    mesh = new MeshNode();
    soil_moisture = new SoilMoisureSensorGroveV1(0);
    Serial.println("adding tasks");
    mesh->add_measurement(soil_moisture->get_measure_callback(), 2);
    mesh->add_send_values(soil_moisture->get_values_callback(), 4);
    #endif
    Serial.println("setup done");
}

void loop(void)
{
    #ifdef ESP32
    update_node();
    #else
    mesh->update();
    #endif
}
