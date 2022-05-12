#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>
// #include "Device.cpp"


struct Measurement{
    bool last = false;
    String type;
    float value;
};

typedef std::function<void()> measure_callback_t;
typedef std::function<Measurement()> get_values_callback_t;

class Sensor{
    private:
        bool enabled = true;

    protected:
        uint8_t power_pin;
        //TODO config this at calibration
        bool calibrated = true;
        Sensor(String HARDWARE_INFO, String TYPE, String UNITS, uint8_t power_pin = -1);

    public:
        const String HARDWARE_INFO;
        const String TYPE;
        const String UNITS; // TODO should be array 

        virtual void measure();
        virtual Measurement get_values(); 
        virtual void calibrate();
        virtual void power_on();
        virtual void power_off();
        virtual void enable();
        virtual void disable();
        virtual measure_callback_t get_measure_callback();
        virtual get_values_callback_t get_values_callback();

};

#endif /* _SENSOR_H_ */