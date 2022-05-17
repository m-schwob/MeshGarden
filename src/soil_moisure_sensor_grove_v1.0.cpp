#include "soil_moisure_sensor_grove_v1.0.h"


// ADS1X15 related. TODO make driver for it. check example file
void SoilMoisureSensorGroveV1::init_adc(){
    ads.setGain(GAIN_ONE);  
    if (!ads.begin()) {  
        Serial.println("Failed to initialize ADS.");
    }
    else
        Serial.println("ADS initialized");
}

float SoilMoisureSensorGroveV1::extender_measure(uint16_t pin){
    int16_t adc0 = ads.readADC_SingleEnded(pin);
    float volts0 = ads.computeVolts(adc0);
    Serial.println("extender measure"); //TODO print value 
    return volts0;
}

//TODO maybe move to constant file
// return percentage, in [0,1] range, of value in range.
// parameter min is optional and the default is 0.
// return -1 in case of error
float SoilMoisureSensorGroveV1::percentage(float value, float max, float min){
    if(min > value || value > max) return -1;
    return (value - min)/(max - min);
}


//TODO remove hard coded. remove default value from analog_pin after solving data saving
SoilMoisureSensorGroveV1::SoilMoisureSensorGroveV1(int id, uint8_t analog_pin, uint8_t power_pin ) 
    : Sensor(id, _DEVICE_TYPE, _HARDWARE_INFO, analog_pin, _MEASUREMENTS_TYPE, _UNITS, power_pin){
    // init I/O power pin if define. init to power off state
    init_adc(); // ADS1X15 related.
    Serial.printf("%s: initelized", HARDWARE_INFO.c_str()); // add details about pins i.e. 
}

void SoilMoisureSensorGroveV1::measure(){
    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        digitalWrite(POWER_PIN, HIGH);

    // TODO solve it when solving extender
    // if(!extender)
    //     analogRead(pin);
    // else
    float volt = extender_measure(analog_pin);
    //TODO handle the case that the value go off range
    result = 1 - percentage(volt, C_air, C_water);
    Serial.printf("%s: measure %f volts, %f/1 range\n", HARDWARE_INFO.c_str(), volt, result);

    // power on sensor if power pin is defined 
    if(POWER_PIN != -1)
        digitalWrite(POWER_PIN, HIGH); //TODO solve it and change to low
}

Measurement SoilMoisureSensorGroveV1::get_values(){
    Measurement measurement;
    measurement.sensor_id = ID;
    measurement.type = MEASUREMENTS_TYPE;
    measurement.value = result;
    measurement.last = true;
    return measurement;
}

void SoilMoisureSensorGroveV1::calibrate(){
    //TODO
    calibrated = true;
}

