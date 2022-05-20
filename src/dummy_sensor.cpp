#include "dummy_sensor.h"


DummySensor::DummySensor(int id, String sensor_type, String measurement_type, String units, uint8_t power_pin)
        : DummySensor(id, std::vector<String>({sensor_type}), std::vector<String>({measurement_type}), std::vector<String>({units}), power_pin) {}

DummySensor::DummySensor(int id, String sensor_type[], String measurement_type[], String units[], uint8_t power_pin)
        : DummySensor(id, std::vector<String>(sensor_type, arr_end(sensor_type)), std::vector<String>(measurement_type, arr_end(measurement_type)),
             std::vector<String>(units, arr_end(units)), power_pin) {}

DummySensor::DummySensor(int id, std::vector<String> sensor_type, std::vector<String> measurements_type, std::vector<String> units, uint8_t power_pin ) 
    : Sensor(id, sensor_type, _HARDWARE_INFO, -1, measurements_type, units, power_pin){
    Serial.printf("%s: initelized", HARDWARE_INFO.c_str()); // add details about pins i.e. 
}

void DummySensor::measure(){
    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        power_on();

    for(int i=0; i < MEASUREMENTS_TYPE.size(); i++) {    
        Measurement measurement;
        measurement.type = MEASUREMENTS_TYPE[i];
        measurement.value = random(0,101)/100;
        measurement.last = false;
        results.push(measurement);
        Serial.printf("%s: measure %d %f %s\n", HARDWARE_INFO.c_str(), MEASUREMENTS_TYPE[i], measurement.value, UNITS[i]);
    }
    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        power_off();
}

Measurement DummySensor::get_values(){
    // TODO consider making iterator for getting values. first time stamp issue must be solved. for now assumes that it never called it on empty queue. 
    if(results.size() == 1)
        results.front().last = true;
    Measurement measurement = results.front();
    results.pop();
    return measurement;
}

void DummySensor::calibrate(){
    //TODO
    calibrated = true;
}

