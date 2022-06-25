#ifndef _MESHGARDEN_H_
#define _MESHGARDEN_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
#include <LittleFS.h>
#include <EEPROM.h>

#include "constants_utils.h"
#include "pins_maps.h"
#include "device.h"
#include "sensor.h"

#ifdef ESP32
#include "mesh_bridge.h"
#else
#include "mesh_node.h"
#endif

#define EEPROM_SIZE 3 * sizeof(int)

uint8_t pin(String pin);

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

    // std::list<Device*> device_list; make private after test
    std::map<String, Funcs> funcs_map;

    String mesh_prefix;
    String mesh_password;
    size_t mesh_port;

#ifdef ESP32
    MeshBridge *network = NULL;
#else
    MeshNode *network = NULL;
#endif

private:
    void store_timing(Time &time, int &sleep_time);
    void load_timing(Time &time, int &sleep_time);
    void save_configuration(String &config);
    bool load_configuration();
    void parse_config();
    void log_config();
    void init_mesh_connection();

public:
    std::list<Device *> device_list;
    MeshGarden();
    void add_sensor(String hardware_id, InitSensor init_sensor_func, Measure measure_func);
    void update();
    void begin();
    int start = 0;
};

#endif /* _MESHGARDEN_H_ */