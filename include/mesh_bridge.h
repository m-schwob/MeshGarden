#ifndef _BRIDGENODE_H_
#define _BRIDGENODE_H_

// BRIDGE NODE Code - has to be ESP32 for now, the node will receive a message from the other nodes
//(for now the message contains 2 values and it is written in the following manner: "<NODE_ID> <NODE COUNTER>")
// the bridge will decode the message, and every time he receives one he will add the values to a map by:
// key: nodeId , value: measurement. every 30 seconds it will disconnect from the mesh, connect to WiFi, and push the last saved data
// start:
#include <Arduino.h>
#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <vector>
#include <list>
#include "ArduinoJson.h"

using namespace painlessmesh;
using namespace std;

/***************************
 *  Macro Definitions For the mesh
 **************************/
#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

/***************************
 *  Variables Definitions For the WiFi - Change wifi ssid,password to match yours
 **************************/
const char* const ssid = "My_hotspot";
const char* const password = "mypassword";
const char* const ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 7200;

/***************************
 *  Macro Definitions For the FireStore DB
 **************************/
#define API_KEY "AIzaSyCpnmHDV7B7oR6pnKXS0VBFoqaF174UzZM"
#define FIREBASE_PROJECT_ID "meshgarden-iot"
#define USER_EMAIL "ioadmin@ioadmin.com"
#define USER_PASSWORD "ioadmin"


class MeshBridge
{

private:
    Scheduler userScheduler; // to control your personal task
    painlessMesh mesh;
    // firebase global variables
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    list<String> change_log;
    // sync with the server, saving data variables
    int lasttime = 0; // initialized, used to messure time interaval for the disconnect
    // std::map<String,vector<String>> dict;
    std::list<String> mesh_values ; 
    std::vector<String> server_data;
    friend void receivedCallback(uint32_t from, String &msg);
    friend void newConnectionCallback(uint32_t nodeId);
    friend void changedConnectionCallback();
    friend void nodeTimeAdjustedCallback(int32_t offset);

    void init_mesh();
    void firebaseInit();
    void firestoreMeshCollectionClear();
    void firestoreMeshCollectionUpdate();
    void firestoreReadChanges();
    void firestoreDataUpdate(String plant_id, String sensor_id, String meas_type, String value);
    vector<String> split(String s, String delimiter);


public:
    MeshBridge();
    void update();
    void get_mesh_nodes();
};

#endif /* _BRIDGENODE_H_ */