#ifndef _BATTERY_LEVEL_SENSOR_H_
#define _BATTERY_LEVEL_SENSOR_H_

#include <ArduinoJson.h>
#include "constants_utils.h"
#include "sensor.h"
#include "mesh_garden.h"

ADC_MODE(ADC_VCC); // enable measure esp8266 supply input voltage

#define BATTERY_LEVEL_KEY "Battery Level"
#define BATTERY_VOLTAGE_KEY "Battery Voltage"
#define INPUT_VOLTAGE_KEY "Input Voltage"

class PowerMonitorSensor : public Sensor
{
private:
    uint8_t analog_pin;
    float full_voltage = 4.2;  // volts
    float drained_voltage = 3; // volts

public:
    PowerMonitorSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

// REGISTER_DEVICE(PowerMonitorSensor, "Battery Level Sensor");

#endif /* _BATTERY_LEVEL_SENSOR_H_ */