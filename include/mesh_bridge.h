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
#include <map>
#include "ArduinoJson.h"
#include <ESP32Time.h>

#define NEXT_DEATH_JSON 96
using namespace painlessmesh;
using namespace std;

/***************************
 *  Macro Definitions For the mesh
 **************************/
// #define MESH_PREFIX "whateverYouLike"
// #define MESH_PASSWORD "somethingSneaky"
// #define MESH_PORT 5555

/***************************
 *  Variables Definitions For the WiFi - Change wifi ssid,password to match yours
 **************************/

// #define ssid = "My_hotspot";
// #define password = "mypassword";
// #define ntp_server = "pool.ntp.org";
// const long gmt_offset_sec = 3600;
// const int daylight_offset_sec = 3600;

/***************************
 *  Macro Definitions For the FireStore DB
 **************************/
// #define API_KEY "AIzaSyCpnmHDV7B7oR6pnKXS0VBFoqaF174UzZM"
// #define FIREBASE_PROJECT_ID "meshgarden-iot"
// #define USER_EMAIL "ioadmin@ioadmin.com"
// #define USER_PASSWORD "ioadmin"

/***************************
 *  Macro Definitions For the RTC and for time management:
 **************************/

class MeshBridge
{

private:
    ESP32Time rtc;
    Scheduler userScheduler; // to control your personal task
    painlessMesh mesh;
    unsigned long myTime; // timer to check how long it takes to initialize mesh network
    Task taskSendMessage;
    // firebase global variables
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    bool initialized = false;
    bool init_death = false;

    String MESH_PREFIX;
    String MESH_PASSWORD;
    unsigned int MESH_PORT;

    char *ssid;
    char *password;
    char *ntp_server;
    long gmt_offset_sec;
    int daylight_offset_sec;

    String API_KEY;
    String FIREBASE_PROJECT_ID;
    String USER_EMAIL;
    String USER_PASSWORD;

    // sync with the server, saving data variables
    int lasttime = 0; // initialized, used to messure time interaval for the disconnect
    // std::map<String,vector<String>> dict;
    std::list<String> mesh_values;
    std::map<String, String> server_data;
    // std::vector<String> server_data;
    friend void receivedCallback(uint32_t from, String &msg);
    friend void newConnectionCallback(uint32_t nodeId);
    friend void changedConnectionCallback();
    friend void nodeTimeAdjustedCallback(int32_t offset);

    // firebase functions
    void firebaseInit();
    void firestoreMeshCollectionClear();
    void firestoreMeshCollectionUpdate();
    void firestoreNetworkDataCollectionUpdate();
    void firestoreReadChanges();
    bool firestoreReadNetwork(String &changes);
    void firestoreDataUpdate(String jsonVal);
    // bool get_node_changes(String node_id, String &changes);
    vector<String> split(String s, String delimiter);

    void set_in_firebase(String nodeId);
    void set_bridge_in_firebase(String nodeId);
    void set_default_nickname(String nodeId);

public:
    bool got_time = false;
    std::map<String, String> change_log;
    MeshBridge();
    void update();
    void get_mesh_nodes();
    void init_clock();
    void init_mesh();
    void set_global_config(JsonObject global_config);
    void exit_mesh_connect_server();
    bool configure_ready = false;
    String config_string;
    vector<String> meassures;
    int die_seconds = 0;
    int die_minutes = 0;
    int die_hours = 0;
    void calculate_death(int ttd);
    ~MeshBridge();
};

#endif /* _BRIDGENODE_H_ */