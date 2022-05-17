#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <Arduino.h>

/* TODO remove later
    data from memory/server:
    - id
    - pins - format (number, I/O, job)? array?
    - power pin and if power enable.
    - type of the device.
*/

class Device {
    private:
        bool enabled = false; // device is disabled when initialized
    public:
        const unsigned int ID;
        const String DEVICE_TYPE; // per device type. e.g. 2 UV sensors will have the same type. // TODO enum or list of types
        const String HARDWARE_INFO; // per implementation info e.g. 2 different uv sensors can hav different info
    private:
        const uint8_t DATA_PINS; // should be an array
    public:
        const uint8_t POWER_PIN; // no power control by default.

    protected:
        Device() = delete; //since the class isn't pure virtual, that will prevent constracting object of this class 
        Device(int id, String device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin = -1);

    public:
        virtual void enable(bool _power_on = false);
        virtual void disable();
        virtual void power_on();
        virtual void power_off();
};

#endif /* _DEVICE_H_ */