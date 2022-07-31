#ifndef _SOIL_MOISTURE_SENSOR_H_
#define _SOIL_MOISTURE_SENSOR_H_

#include "constants_utils.h"
#include "pins_maps.h"
#include "Device.h"
#include "Sensor.h"

#define CAPACITIVE_SOIL_MOISTURE_SENSOR "Capacitive Soil Moisture Sensor"

#define SOIL_MOISTURE_KEY "Soil Moisture"

class SoilMoistureSensor : public Sensor
{

private:
    uint8_t analog_pin = 0;

    float C_air = 2.2; // volts
    float C_water = 1; // volts

public:
    SoilMoistureSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

REGISTER_DEVICE(SoilMoistureSensor, CAPACITIVE_SOIL_MOISTURE_SENSOR);

#endif /* _SOIL_MOISTURE_SENSOR_H_ */