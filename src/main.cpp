#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_ADS1X15.h>
//DO NOT REMOVE INCLUDES UNLESS UPDATING INI FILE!!!!

#include "soil_moisure_sensor_grove_v1.0.cpp"

#define BAUD_RATE 115200 

void setup(void)
{
  Serial.begin(BAUD_RATE);
  Serial.println("Hello!");

}

void loop(void)
{
  
  delay(1000);
}