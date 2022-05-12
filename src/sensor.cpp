#include "sensor.h"


Sensor::Sensor(String HARDWARE_INFO, String TYPE, String UNITS, uint8_t power_pin)
    : power_pin(power_pin), HARDWARE_INFO(HARDWARE_INFO), TYPE(TYPE), UNITS(UNITS) {}

void Sensor::power_on(){
    //TODO
}
void Sensor::power_off(){
    //TODO
}

void Sensor::enable(){
    enabled = true;
    power_on(); //TODO maybe enable should not power on?
}
void Sensor::disable(){
    enabled = false;
    power_off();
}

measure_callback_t Sensor::get_measure_callback(){
    return [this](){measure();};
}

get_values_callback_t Sensor::get_values_callback(){
    return [this]()->Measurement{return get_values();};
}