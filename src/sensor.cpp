#include "sensor.h"


Sensor::Sensor(int id, String device_type, String hardware_info, uint8_t data_pins, String measurements_type, String units, uint8_t power_pin)
    : Device(id, device_type, hardware_info, data_pins, power_pin), MEASUREMENTS_TYPE(measurements_type), UNITS(units) {}

measure_callback_t Sensor::get_measure_callback(){
    return [this](){measure();};
}

get_values_callback_t Sensor::get_values_callback(){
    return [this]()->Measurement{return get_values();};
}