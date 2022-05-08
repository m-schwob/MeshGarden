#include <Arduino.h>
#include "Device.cpp"


struct Measurement{
    String key;
    float value;
};

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