#include <Arduino.h>
#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_ADS1X15.h>
//DO NOT REMOVE INCLUDES UNLESS UPDATING INI FILE!!!!

#if defined(ESP32)
#include "mesh_bridge.h"
#elif defined(ESP8266)
#include "mesh_node.h"
#endif 
// #include "soil_moisure_sensor_grove_v1.0.cpp"

#define BAUD_RATE 115200 

MeshNode node;

void setup(void)
{
  Serial.begin(BAUD_RATE);
  Serial.println("Hello1!");
  Serial.println("Hello2!");

}

void loop(void)
{
  node.update();
}

