#include "library_devices/power_monitor_sensor.h"

const float PowerMonitorSensor::full_voltage = 4.2;  // volts
const float PowerMonitorSensor::drained_voltage = 3; // volts
const int PowerMonitorSensor::map_steps = 13;
const float PowerMonitorSensor::map_step = 0.1;
const int PowerMonitorSensor::capacity_map[map_steps] = {0, 1, 2, 5, 13, 22, 39, 53, 62, 74, 84, 94, 100};

// battery  is not linear. estimate ther percentage with the help of the capacity map
float PowerMonitorSensor::calculate_percentages(float voltage)
{
    float index = (voltage - drained_voltage) / map_step;
    int upper_index = ceil(index);
    int lower_index = floor(index);

    Serial.println("PowerMonitorSensor: indexes i-" + String(index) + "u-" + String(upper_index) + "l-" + String(lower_index));
    return 0.5 * (capacity_map[lower_index] + capacity_map[upper_index]); // avg: lower + (upper-lower)/2 => 0.5*lower + 0.5*upper => 0.5*(upper+lower)
}

// measured voltage (VOUT) is not the real battery voltage since there is voltage divider
// VBAT -> R1 -> VOUT -> R2 -> GND
// TODO check if input voltage ha effect on measurement
float PowerMonitorSensor::real_voltage(float VOUT)
{
    int R1 = 36000, R2 = 100000; // ohm
    return (1 + R2 / R1) * VOUT;
}

PowerMonitorSensor::PowerMonitorSensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : Sensor(device_id, hardware_info, pinout, envelop)
{
    if (pinout.containsKey("BAT"))
        analog_pin = pin(pinout["BAT"].as<String>());
    else
    {
        Serial.print(HARDWARE_INFO.c_str());
        Serial.println(":Warning: sensor pin not defined. set to 0 as default");
    }
}

Measurements PowerMonitorSensor::measure()
{
    Measurements measurements(3);

    Measurement battery_level;
    battery_level.type = BATTERY_LEVEL_KEY;
    float volt = analog_read(analog_pin);
    battery_level.value = 100 * calculate_percentages(real_voltage(volt)); // TODO calculate by non linear graph
    measurements.push_back(battery_level);

    Measurement battery_voltage;
    battery_voltage.type = BATTERY_VOLTAGE_KEY;
    battery_voltage.value = volt;
    measurements.push_back(battery_voltage);

    Measurement input_voltage;
    input_voltage.type = INPUT_VOLTAGE_KEY;
    input_voltage.value = ESP.getVcc() / 10; // getVcc return voltage in mV
    measurements.push_back(input_voltage);

    String s = HARDWARE_INFO + ": " +
               "battery voltage " + String(battery_voltage.value) + "V, " +
               "input voltage " + String(input_voltage.value) + "V, " +
               "battery level " + String(battery_level.value) + "V, ";
    Serial.println(s);
    return measurements;
}

void PowerMonitorSensor::calibrate() { calibrated = true; }