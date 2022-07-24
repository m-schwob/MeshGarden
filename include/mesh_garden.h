#ifndef _MESHGARDEN_H_
#define _MESHGARDEN_H_

#include <Arduino.h>
#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "device.h"
#include "sensor.h"

#if defined(ESP32)
    #include "mesh_bridge.h"
    MeshBridge* mesh = NULL;

#elif defined(ESP8266)
    #include "mesh_node.h"
    #include "soil_moisure_sensor_grove_v1.0.h"
    #include "dummy_sensor.h"
#endif

#define BAUD_RATE 115200


class MeshGarden{
    private:
        DynamicJsonDocument config;
        std::list<Device*> device_list; 
        String mesh_prefix;
        String mesh_password;
        size_t mesh_port;

    private:
        void save_configuration(String& config);
        bool load_configuration();
        void parse_config();

    public:
        MeshGarden();
        void update();

};

#endif /* _MESHGARDEN_H_ */