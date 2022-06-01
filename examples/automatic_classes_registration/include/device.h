#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <Arduino.h>
#include <vector>
#include <map>


/* TODO remove later
    data from memory/server:
    - id
    - pins - format (number, I/O, job)? array?
    - power pin and if power enable.
    - type of the device.
*/

inline String* arr_end(String* arr){
    return arr + sizeof(arr)/sizeof(String);
}

class Device {
    private:
        bool enabled = false; // device is disabled when initialized
    public:
        const unsigned int ID;
        const std::vector<String> DEVICE_TYPE; // per device type. e.g. 2 UV sensors will have the same type. // TODO enum or list of types
        const String HARDWARE_INFO; // per implementation info e.g. 2 different uv sensors can hav different info
    private:
        const uint8_t DATA_PINS; // should be an array
    public:
        const uint8_t POWER_PIN; // no power control by default.

    protected:
        Device() = delete; //since the class isn't pure virtual, that will prevent constracting object of this class 
        Device(int id, String device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin = -1);
        Device(int id, String device_type[], String hardware_info, uint8_t data_pins, uint8_t power_pin = -1);
        Device(int id, std::vector<String> device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin = -1);

    public:
        virtual void enable(bool _power_on = false);
        virtual void disable();
        virtual void power_on();
        virtual void power_off();
        virtual ~Device() {} //not sure if needed. added to follow DeviceFactory original code reference
};


struct DeviceFactory
{
    static Device *create(int id, std::vector<String> device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin)
    { // creates an object from a string
        const Creators_t::const_iterator iter = static_creators().find(hardware_info);
        if (iter == static_creators().end())
        {
            Serial.println("Factory: '" + hardware_info + "' does not match any device.");
            return 0;
        }
        else
            return (*iter->second)(id, device_type, hardware_info, data_pins, power_pin); // if found, execute the creator function pointer
    }

private:
    typedef Device *Creator_t(int id, std::vector<String> device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin);                      // function pointer to create Device
    typedef std::map<String, Creator_t *> Creators_t; // map from id to creator
    static Creators_t &static_creators()
    {
        static Creators_t s_creators;
        return s_creators;
    } // static instance of map
    template <class T = int>
    struct Register
    {
        static Device *create(int id, std::vector<String> device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin) 
        { return new T(id, device_type, hardware_info, data_pins, power_pin); };
        static Creator_t *init_creator(const String &id) { return static_creators()[id] = create; }
        static Creator_t *creator;
    };
};

#define REGISTER_DEVICE(T, STR) template <> \
                                DeviceFactory::Creator_t *DeviceFactory::Register<T>::creator = DeviceFactory::Register<T>::init_creator(STR)


#endif /* _DEVICE_H_ */