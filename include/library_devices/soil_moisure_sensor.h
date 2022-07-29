#ifndef _SOIL_MOISTURE_SENSOR_H_
#define _SOIL_MOISTURE_SENSOR_H_

#include "constants_utils.h"
#include "pins_maps.h"
#include "Device.h"
#include "Sensor.h"

#define SOIL_MOISTURE_KEY "Soil Moisture"

class SoilMoistureSensor : public Sensor
{
public:
    static const String HARDWARE_INFO;

private:
    uint8_t analog_pin = 0;

    float C_air = 2.2; // volts
    float C_water = 1; // volts

public:
    SoilMoistureSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

REGISTER_DEVICE(SoilMoistureSensor, SoilMoistureSensor::HARDWARE_INFO);

#endif /* _SOIL_MOISTURE_SENSOR_H_ */