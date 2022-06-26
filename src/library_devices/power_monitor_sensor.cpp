#include "library_devices/power_monitor_sensor.h"

PowerMonitorSensor::PowerMonitorSensor(DEVICE_CONSTRUCTOR_ARGUMENTS) : Sensor(device_id, hardware_info, pinout, envelop)
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
    battery_level.value = 100 * percentage(volt, full_voltage, drained_voltage); // TODO calculate by non linear graph
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