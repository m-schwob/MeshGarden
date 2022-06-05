#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>
#include <vector>
#include <functional>
#include "Device.h"


struct Measurement{
    unsigned int sensor_id;
    bool last = false;
    String type;
    float value;
};

typedef std::function<void()> measure_callback_t;
typedef std::function<Measurement()> get_values_callback_t;

class Sensor : public Device{
    protected:
        //TODO config this at calibration
        bool calibrated = true;
        Sensor() = delete;
        Sensor(int id, String sensor_type, String hardware_info, uint8_t data_pins, String measurement_type, String units, uint8_t power_pin = -1);
        Sensor(int id, String sensor_type[], String hardware_info, uint8_t data_pins, String measurement_type[], String units[], uint8_t power_pin = -1);
        Sensor(int id, std::vector<String> sensor_type, String hardware_info, uint8_t data_pins, std::vector<String> measurement_type, std::vector<String> units, uint8_t power_pin = -1);

    public:
        const std::vector<String> MEASUREMENTS_TYPE; // identifier for what is being measured. e.g. humidity, temperature // TODO should be array 
        const std::vector<String> UNITS; // the units of the measurements. e.g. voltage, percentage, Celsius // TODO should be array 

        virtual Measurement measure() = 0;
        // virtual void measure() = 0;
        // virtual Measurement get_values() = 0; 
        virtual void calibrate() = 0;
        virtual measure_callback_t get_measure_callback();
        // virtual get_values_callback_t get_values_callback();

};

#endif /* _SENSOR_H_ */