#include <Arduino.h>
#include "Device.cpp"

//REMOVE LATER
#include "soil_moisure_sensor_grove_v1.0.h"

// struct Measurement{
//     bool last = false;
//     String type;
//     float value;
// };

class Sensor:Device{
    private:

    public:
        int n;
        const String UNITS; // TODO should be array 

        Sensor(){};
        virtual void enable();
        virtual void disable();
        virtual void measure();
        virtual Measurement get_values(); 
        virtual void calibrate();
        virtual void power_on();
        virtual void power_off();
};