#include "sensor.h"


Sensor::Sensor(int id, String sensor_type, String hardware_info, uint8_t data_pins, String measurement_type, String units, uint8_t power_pin)
        : Sensor(id, std::vector<String>({sensor_type}), hardware_info, data_pins, std::vector<String>({measurement_type}), std::vector<String>({units}), power_pin) {}

// TODO validate inputs sizes
Sensor::Sensor(int id, String sensor_type[], String hardware_info, uint8_t data_pins, String measurement_type[], String units[], uint8_t power_pin)
        : Sensor(id, std::vector<String>(sensor_type, arr_end(sensor_type)), hardware_info, data_pins, std::vector<String>(measurement_type, arr_end(measurement_type)), 
            std::vector<String>(units, arr_end(units)), power_pin) {}

// TODO validate inputs sizes
Sensor::Sensor(int id, std::vector<String> sensor_type, String hardware_info, uint8_t data_pins, std::vector<String> measurements_type, std::vector<String> units, uint8_t power_pin)
    : Device(id, sensor_type, hardware_info, data_pins, power_pin), MEASUREMENTS_TYPE(measurements_type), UNITS(units) {}

measure_callback_t Sensor::get_measure_callback(){
    return [this](){measure();};
}

// get_values_callback_t Sensor::get_values_callback(){
//     return [this]()->Measurement{return get_values();};
// }