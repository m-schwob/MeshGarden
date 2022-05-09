#include <Arduino.h>
#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_ADS1X15.h>
//DO NOT REMOVE INCLUDES UNLESS UPDATING INI FILE!!!!

#include "soil_moisure_sensor_grove_v1.0.cpp"
//#include "sensor.cpp"

#define BAUD_RATE 115200
SoilMoisureSensorGroveV1 soil_moisture;

void setup(void)  
{
    Serial.begin(BAUD_RATE);
    Serial.println("Hello!");
    SoilMoisureSensorGroveV1 soil_moisture(0,D3);
}

void loop(void)
{
    soil_moisture.measure();
    Measurement meas = soil_moisture.get_values();
    printf("%s: %f%%\n", meas.key.c_str(), meas.value*100);
    delay(2000);
}
