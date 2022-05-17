#include "dummy_sensor.h"


//TODO remove hard coded. remove default value from analog_pin after solving data saving
DummySensor::DummySensor(int id, String sensor_type, String measurement_type, String units, uint8_t power_pin ) 
    : Sensor(id, sensor_type, _HARDWARE_INFO, -1, measurement_type, units, power_pin){
    Serial.printf("%s: initelized", HARDWARE_INFO.c_str()); // add details about pins i.e. 
}

void DummySensor::measure(){
    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        power_on();

    result = random(0,101)/100;
    Serial.printf("%s: measure %s %f %s\n", HARDWARE_INFO.c_str(), MEASUREMENTS_TYPE.c_str(), result, UNITS.c_str());

    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        power_off();
}

Measurement DummySensor::get_values(){
    Measurement measurement;
    measurement.sensor_id = ID;
    measurement.type = MEASUREMENTS_TYPE;
    measurement.value = result;
    measurement.last = true;
    return measurement;
}

void DummySensor::calibrate(){
    //TODO
    calibrated = true;
}

