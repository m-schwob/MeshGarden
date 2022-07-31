#include <Arduino.h>
#include <painlessMesh.h>
// DO NOT REMOVE INCLUDES UNLESS UPDATING INI FILE!!!!

#if defined(ESP32)
    #include "mesh_bridge.h"
    MeshBridge *meshB = NULL;
#elif defined(ESP8266)
    #include "mesh_node.h"
    MeshNode *meshB = NULL;
#endif

#define BAUD_RATE 115200


void setup(void)
{
    Serial.begin(BAUD_RATE);
    Serial.println();
    Serial.println(); // make lines space in the begining
#ifdef ESP32
    meshB = new MeshBridge();
#else
    meshB = new MeshNode();
#endif
    Serial.println("setup done");

}

void loop(void)
{
    meshB->update();
}
