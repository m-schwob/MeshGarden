#include "library_devices/power_monitor_sensor.h"

const float PowerMonitorSensor::full_voltage = 4.2;  // volts
const float PowerMonitorSensor::drained_voltage = 3; // volts
const int PowerMonitorSensor::map_steps = 13;
const float PowerMonitorSensor::map_step = 0.1;
const int PowerMonitorSensor::capacity_map[map_steps] = {0, 1, 2, 5, 13, 22, 39, 53, 62, 74, 84, 94, 100};

// battery  is not linear. estimate ther percentage with the help of the capacity map
//return 0-100 float value
float PowerMonitorSensor::calculate_percentages(float voltage)
{
    float index = (voltage - drained_voltage) / map_step;
    int upper_index = ceil(index) >= map_steps? map_step: ceil(index);
    int lower_index = floor(index) < 0? 0: floor(index);

    Serial.println("PowerMonitorSensor: indexes-> i-" + String(index) + ", u-" + String(upper_index) + ", l-" + String(lower_index));
    // linear calculation between map indexes values
    return capacity_map[lower_index] + (index - lower_index) * (capacity_map[upper_index] - capacity_map[lower_index]);
    // return 0.5 * (capacity_map[lower_index] + capacity_map[upper_index]); // avg: lower + (upper-lower)/2 => 0.5*lower + 0.5*upper => 0.5*(upper+lower)
}

// measured voltage (VOUT) is not the real battery voltage since there is voltage divider
// VBAT -> R1 -> VOUT -> R2 -> GND
// TODO check if input voltage ha effect on measurement
float PowerMonitorSensor::real_voltage(float VOUT)
{
    float R1 = 36000.0, R2 = 100000.0; // ohm
    float _real_voltage = (1 + R1 / R2) * VOUT;
    // Serial.println("VOUT: " + String(VOUT) + " real voltage calculated: " + String(_real_voltage));
    return _real_voltage;
}

PowerMonitorSensor::PowerMonitorSensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : Sensor(device_id, hardware_info, pinout, envelop)
{
    if (pinout.containsKey("BAT"))
        analog_pin = pin(pinout["BAT"].as<String>());
    else
    {
        Serial.print(HARDWARE_INFO.c_str());
        // TODO consider not setting default pin but disabling it
        Serial.println(":Warning: sensor pin not defined. set to 0 as default");
    }
}

Measurements PowerMonitorSensor::measure()
{
    String s = HARDWARE_INFO + ": ";
// until i will find esp32 equivalent for ESP.getVcc()
#if defined(ESP8266)
    Measurements measurements(3);

    Measurement input_voltage;
    input_voltage.type = INPUT_VOLTAGE_KEY;
    input_voltage.value = ESP.getVcc() / 1000; // getVcc return voltage in mV
    measurements.push_back(input_voltage);

    s += "input voltage " + String(input_voltage.value) + "V, ";
#else
    Measurements measurements(2);
#endif

    float volt = analog_read(analog_pin);

    Measurement battery_voltage;
    battery_voltage.type = BATTERY_VOLTAGE_KEY;
    battery_voltage.value = real_voltage(volt);
    measurements.push_back(battery_voltage);

    Measurement battery_level;
    battery_level.type = BATTERY_LEVEL_KEY;
    battery_level.value = calculate_percentages(battery_voltage.value); // TODO calculate by non linear graph
    measurements.push_back(battery_level);

    s += "battery voltage " + String(battery_voltage.value) + "V, " +
         "battery level " + String(battery_level.value) + "%";
    Serial.println(s);
    return measurements;
}

void PowerMonitorSensor::calibrate() { calibrated = true; }