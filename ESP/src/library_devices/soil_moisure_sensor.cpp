#include "library_devices/soil_moisure_sensor.h"

SoilMoistureSensor::SoilMoistureSensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : Sensor(device_id, hardware_info, pinout, envelop)
{

    // temp fix
#if defined(ESP8266)
    pinMode(D5, OUTPUT);
    digitalWrite(D5, HIGH);
#endif

    if (pinout.containsKey("AOUT"))
        analog_pin = pin(pinout["AOUT"].as<String>());
    else
    {
        Serial.print(HARDWARE_INFO.c_str());
        // TODO consider not setting default pin but disabling it
        Serial.println(":Warning: sensor pin not defined. set to 0 as default");
    }
}

Measurements SoilMoistureSensor::measure()
{
    Measurements measurements(1);
    Measurement moisture;
    moisture.type = SOIL_MOISTURE_KEY;

    float volt = analog_read(analog_pin);
    // TODO handle the case that the value go off range
    moisture.value = 100 - percentage(volt, C_air, C_water);
    measurements.push_back(moisture);
    Serial.printf("%s: measure %f volts, %f/1 range\n", HARDWARE_INFO.c_str(), volt, moisture.value);
    return measurements;
}

void SoilMoistureSensor::calibrate() { calibrated = true; }