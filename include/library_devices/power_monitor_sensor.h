#ifndef _BATTERY_LEVEL_SENSOR_H_
#define _BATTERY_LEVEL_SENSOR_H_

#include <ArduinoJson.h>
#include "constants_utils.h"
#include "pins_maps.h"
#include "sensor.h"

// ADC_MODE(ADC_VCC); // enable measure esp8266 supply input voltage

#define BATTERY_LEVEL_KEY "Battery Level"
#define BATTERY_VOLTAGE_KEY "Battery Voltage"
#define INPUT_VOLTAGE_KEY "Input Voltage"

class PowerMonitorSensor : public Sensor
{
private:
    uint8_t analog_pin = 0;

    static const float full_voltage;    // volts
    static const float drained_voltage; // volts
    static const int map_steps;
    static const float map_step;
    static const int capacity_map[];

    static float calculate_percentages(float voltage);
    static float real_voltage(float voltage);

public:
    PowerMonitorSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

// REGISTER_DEVICE(PowerMonitorSensor, "Battery Level Sensor");

#endif /* _BATTERY_LEVEL_SENSOR_H_ */