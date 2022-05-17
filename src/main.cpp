#include <Arduino.h>
#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_ADS1X15.h>
// DO NOT REMOVE INCLUDES UNLESS UPDATING INI FILE!!!!


#if defined(ESP32)
    #include "mesh_bridge.h"
    MeshBridge* mesh = NULL;

#elif defined(ESP8266)
    #include "mesh_node.h"
    #include "soil_moisure_sensor_grove_v1.0.h"
    #include "dummy_sensor.h"
    MeshNode* mesh = NULL;
    SoilMoisureSensorGroveV1* soil_moisture = NULL;
    DummySensor* dummy_sensor = NULL;
#endif


#define BAUD_RATE 115200


void setup(void)
{
    Serial.begin(BAUD_RATE);
    Serial.println(); Serial.println(); // make lines space in the begining
    #ifdef ESP32
        mesh = new MeshBridge();
    #else
        mesh = new MeshNode();
        soil_moisture = new SoilMoisureSensorGroveV1(1,0);
        dummy_sensor = new DummySensor(0, "Temperature", "Temperature", "Celsius");
        soil_moisture->enable(true);
        dummy_sensor->enable(true);
        Serial.println("adding tasks");
        mesh->add_measurement(soil_moisture->get_measure_callback(), 2);
        mesh->add_send_values(soil_moisture->get_values_callback(), 4);
        mesh->add_measurement(dummy_sensor->get_measure_callback(), 2);
        mesh->add_send_values(dummy_sensor->get_values_callback(), 4);

    #endif
    Serial.println("setup done");
}

void loop(void)
{
    mesh->update();
}
