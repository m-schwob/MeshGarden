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
    DummySensor* dummy_sensor1 = NULL;
    DummySensor* dummy_sensor2 = NULL;
    DummySensor* dummy_sensor3 = NULL;
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
        soil_moisture = new SoilMoisureSensorGroveV1(0,0);

        // init single measurement type dummy sensor 
        dummy_sensor1 = new DummySensor(1, "Temperature", "Temperature", "Celsius");

        // init multiple measurement types dummy sensor using {...} methode (or pointer to array)
        // NOTE!!! this init method made for convenience but im not sure it works XD. be careful.
        dummy_sensor2 = new DummySensor(2, {"Temperature", "Humidity"}, {"Temperature", "Humidity"}, {"Celsius", "%"});

        // init multiple measurement types dummy sensor directly using std::vector<String>. this is more robust init methode
        dummy_sensor3 = new DummySensor(3, std::vector<String>({"Temperature", "Humidity"}), std::vector<String>({"Temperature", "Humidity"}), std::vector<String>({"Celsius", "%"}));


        soil_moisture->enable(true);
        dummy_sensor1->enable(true);
        dummy_sensor2->enable(true);
        dummy_sensor3->enable(true);


        Serial.println("adding tasks");
        mesh->add_measurement(soil_moisture->get_measure_callback(), 2);
        mesh->add_send_values(soil_moisture->get_values_callback(), 4);

        mesh->add_measurement(dummy_sensor1->get_measure_callback(), 2);
        mesh->add_send_values(dummy_sensor1->get_values_callback(), 4);

        mesh->add_measurement(dummy_sensor2->get_measure_callback(), 2);
        mesh->add_send_values(dummy_sensor2->get_values_callback(), 4);

        mesh->add_measurement(dummy_sensor3->get_measure_callback(), 2);
        mesh->add_send_values(dummy_sensor3->get_values_callback(), 4);
    #endif
    Serial.println("setup done");
}

void loop(void)
{
    mesh->update();
}
