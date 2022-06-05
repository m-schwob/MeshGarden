#ifndef _MESHGARDEN_H_
#define _MESHGARDEN_H_

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <list>

#include "pins_maps.h"
#include "device.h"
#include "sensor.h"

uint8_t pin(String pin);

class MeshGarden
{
public:
    typedef void InitSensor();
    typedef Measurement Measure();
    struct Funcs
    {
        InitSensor init_sensor_func;
        Measure measure_func;
    };

private:
    DynamicJsonDocument config;

    std::list<Device *> device_list;
    std::map<String, Funcs> funcs_map;

    String mesh_prefix;
    String mesh_password;
    size_t mesh_port;

private:
    void save_configuration(String &config);
    bool load_configuration();
    void parse_config();

public:
    MeshGarden();
    void add_sensor(String hardware_id, InitSensor init_sensor_func, Measure measure_func);
    void update();
    void begin();

    // Generic sensor class that can call 'init_sensor' and 'measure' function from 'MeshGarden' list.
    // That enables the easy function registration proccess without breaking DeviceFactory initialization proccess
    class GenericSensor : Sensor
    {
    private:
        InitSensor init_sensor_func;
        Measure measure_func;

    public:
        void init_sensor();
        Measurement measure();
    };
};

#endif /* _MESHGARDEN_H_ */