#include "sensor.h"

Sensor::Sensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : Device(device_id, hardware_info, pinout, create_envelop(DEVICE_TYPE_SENSOR)),
      MEASUREMENTS_TYPE(from_envelop(envelop, MEASUREMENT_TYPE_KEY)), UNITS(from_envelop(envelop, UNITS_KEY)) {}

std::vector<String> Sensor::from_envelop(DynamicJsonDocument envelop, String key)
{
    JsonArray arr = envelop[key];
    std::vector<String> vec(arr.size());
    for (size_t i = 0; i < arr.size(); i++)
    {
        vec[i] = arr[i].as<String>();
    }
    return vec;
}

// Sensor::Sensor(int id, String sensor_type, String hardware_info, uint8_t data_pins, String measurement_type, String units, uint8_t power_pin)
//         : Sensor(id, std::vector<String>({sensor_type}), hardware_info, data_pins, std::vector<String>({measurement_type}), std::vector<String>({units}), power_pin) {}

// // TODO validate inputs sizes
// Sensor::Sensor(int id, String sensor_type[], String hardware_info, uint8_t data_pins, String measurement_type[], String units[], uint8_t power_pin)
//         : Sensor(id, std::vector<String>(sensor_type, arr_end(sensor_type)), hardware_info, data_pins, std::vector<String>(measurement_type, arr_end(measurement_type)),
//             std::vector<String>(units, arr_end(units)), power_pin) {}

// // TODO validate inputs sizes
// Sensor::Sensor(int id, std::vector<String> sensor_type, String hardware_info, uint8_t data_pins, std::vector<String> measurements_type, std::vector<String> units, uint8_t power_pin)
//     : Device(id, _DEVICE_TYPE, hardware_info, data_pins, power_pin), MEASUREMENTS_TYPE(measurements_type), UNITS(units) {}

Measurements Sensor::measure_wrapper(){
    Measurements measurements = measure();
    for(Measurement meas : measurements){
        meas.sensor_id = DEVICE_ID;
    }
    return measurements;
}

measure_callback_t Sensor::get_measure_callback()
{
    return [this]() -> Measurements { return measure(); };
}

// get_values_callback_t Sensor::get_values_callback(){
//     return [this]()->Measurement{return get_values();};
// }