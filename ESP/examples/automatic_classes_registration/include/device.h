#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <Arduino.h>
#include <vector>
#include <map>
#include <ArduinoJson.h>


/* TODO remove later
    data from memory/server:
    - id
    - pins - format (number, I/O, job)? array?
    - power pin and if power enable.
    - type of the device.
*/

// inline String* arr_end(String* arr){
//     return arr + sizeof(arr)/sizeof(String);
// }

#define DEVICE_CONSTRUCTOR_ARGUMENTS const int device_id, const String hardware_info, const JsonObject pinout, const DynamicJsonDocument envelop
#define POWER_PIN F("POWER")
#define DEVICE_TYPE_KEY F("device_type")

class Device {
    private:
        bool enabled = false; // device is disabled when initialized
    public:
        const unsigned int DEVICE_ID; // per instance identification
        const String DEVICE_TYPE; // sensor/actuator ...
        const String HARDWARE_INFO; // per implementation info e.g. 2 different uv sensors can hav different info
    protected:
        const DynamicJsonDocument PINOUT; 
        const bool POWER_PIN_CONTROL; //no power control by default.

    protected:
        Device() = delete; //since the class isn't pure virtual, that will prevent constracting object of this class 
        Device(DEVICE_CONSTRUCTOR_ARGUMENTS);
        // Device(int id, String device_type[], String hardware_info, DynamicJsonDocument pinout);
        // Device(int id, std::vector<String> device_type, String hardware_info, DynamicJsonDocument pinout);

        static DynamicJsonDocument create_envelop(String device_type);

    public:
        virtual void enable(bool _power_on = false);
        virtual void disable();
        virtual void power_on();
        virtual void power_off();
        virtual ~Device() {} //not sure if needed. added to follow DeviceFactory original code reference
};


struct DeviceFactory
{
    static Device *create(DEVICE_CONSTRUCTOR_ARGUMENTS)
    { // creates an object from a string
        const Creators_t::const_iterator iter = static_creators().find(hardware_info);
        if (iter == static_creators().end())
        {
            Serial.println("Factory: '" + hardware_info + "' does not match any device.");
            return 0;
        }
        else
            return (*iter->second)(device_id, hardware_info, pinout, envelop); // if found, execute the creator function pointer
    }

private:
    typedef Device *Creator_t(DEVICE_CONSTRUCTOR_ARGUMENTS);                      // function pointer to create Device
    typedef std::map<String, Creator_t *> Creators_t; // map from id to creator
    static Creators_t &static_creators()
    {
        static Creators_t s_creators;
        return s_creators;
    } // static instance of map
    template <class T = int>
    struct Register
    {
        static Device *create(DEVICE_CONSTRUCTOR_ARGUMENTS) 
        { return new T(device_id, hardware_info, pinout, envelop); };
        static Creator_t *init_creator(const String &id) { return static_creators()[id] = create; }
        static Creator_t *creator;
    };
};

#define REGISTER_DEVICE(T, STR) template <> \
                                DeviceFactory::Creator_t *DeviceFactory::Register<T>::creator = DeviceFactory::Register<T>::init_creator(STR)


#endif /* _DEVICE_H_ */