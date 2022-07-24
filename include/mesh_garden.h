#ifndef _MESHGARDEN_H_
#define _MESHGARDEN_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
#include <LittleFS.h>
#include <EEPROM.h>

#include "constants_utils.h"
#include "pins_maps.h"
#include "library_devices/ADS1X15.h"
#include "library_devices/power_monitor_sensor.h"
#include "device.h"
#include "sensor.h"

#ifdef ESP32
#include "mesh_bridge.h"
#else
#include "mesh_node.h"
#endif

class MeshGarden
{
public:
    typedef std::function<void()> InitSensor;
    typedef std::function<Measurements()> Measure;
    struct Funcs
    {
        InitSensor init_sensor_func;
        Measure measure_func;
    };

    // Generic sensor class that can call 'init_sensor' and 'measure' function from 'MeshGarden' list.
    // That enables the easy function registration proccess without breaking DeviceFactory initialization proccess
    class GenericSensor : public Sensor
    {
    private:
        InitSensor init_sensor_func;
        Measure measure_func;

    public:
        GenericSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
        void init_sensor();
        Measurements measure();
        void calibrate();

        void set(Funcs func);
    };
#define REGISTER_SENSOR(STR) REGISTER_DEVICE(MeshGarden::GenericSensor, STR);

private:
    DynamicJsonDocument config;
    std::list<Device *> device_list;
    std::map<String, Funcs> funcs_map;
    PowerMonitorSensor *power_monitor = NULL;

#ifdef ESP32
    MeshBridge *network = NULL;
#else
    MeshNode *network = NULL;
#endif

private:
    void save_configuration(String &config);
    bool load_configuration();
    void parse_config();
    void log_config();
    void init_power_monitor();
    void init_mesh_connection();

public:
    MeshGarden();
    void add_sensor(String hardware_id, InitSensor init_sensor_func, Measure measure_func);
    std::list<String> get_device_list();
    void update();
    void begin();
    int start = 0;
};

#endif /* _MESHGARDEN_H_ */