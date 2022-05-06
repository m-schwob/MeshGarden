#include <Arduino.h>

/* data from memory/server:
    - id
    - pins - format (number, I/O, job)? array?
    - power pin and if power enable.
    - type of the device.


*/
class Device{
    private:
        const int POWER_PIN;
        const bool POWER_CONTROL;
        const int DATA_PINS; // should be an array

    public:
        const String HARDWARE_INFO; // per implementation info e.g. 2 different uv sensors can hav different info
        // TODO enum of types
        const int DEVICE_TYPE;  // per device type. e.g. 2 UV sensors will have the same type
        const int ID;
        

        Device();
        virtual void enable();
        virtual void disable();
        virtual void power_on();
        virtual void power_off();
};