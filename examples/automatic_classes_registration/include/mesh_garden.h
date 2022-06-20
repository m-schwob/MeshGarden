#ifndef _MESHGARDEN_H_
#define _MESHGARDEN_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
#include <LittleFS.h>

#include "pins_maps.h"
#include "device.h"
#include "sensor.h"


uint8_t pin(String pin);


class MeshGarden
{
public:
    typedef void InitSensor();
    typedef Measurement Measure();

private:
    DynamicJsonDocument config;
    std::list<Device *> device_list;
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
};

#endif /* _MESHGARDEN_H_ */