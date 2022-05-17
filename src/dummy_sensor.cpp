#include "dummy_sensor.h"


//TODO remove hard coded. remove default value from analog_pin after solving data saving
DummySensor::DummySensor(int id, std::vector<String> sensor_type, std::vector<String> measurement_type, std::vector<String> units, uint8_t power_pin ) 
    : Sensor(id, sensor_type, _HARDWARE_INFO, -1, measurement_type, units, power_pin){
    Serial.printf("%s: initelized", HARDWARE_INFO.c_str()); // add details about pins i.e. 
}

void DummySensor::measure(){
    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        power_on();

    for(auto it = MEASUREMENTS_TYPE.begin(); it != MEASUREMENTS_TYPE.end(); ++it) {    
        float result = random(0,101)/100;
        results.push_back(result);
        Serial.printf("%s: measure %d %f %s\n", HARDWARE_INFO.c_str(), *it, result, UNITS);
    }
    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        power_off();
}

Measurement DummySensor::get_values(){
    Measurement measurement;
    measurement.type = MEASUREMENTS_TYPE.erase(MEASUREMENTS_TYPE.begin());
    measurement.value = results.erase(MEASUREMENTS_TYPE.begin());
    measurement.last = true;
    return measurement;
}

void DummySensor::calibrate(){
    //TODO
    calibrated = true;
}

