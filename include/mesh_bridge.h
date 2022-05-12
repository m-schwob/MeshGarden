//BRIDGE NODE Code - has to be ESP32 for now, the node will receive a message from the other nodes 
//(for now the message contains 2 values and it is written in the following manner: "<NODE_ID> <NODE COUNTER>")
//the bridge will decode the message, and every time he recieves one he will add the values to a map by:
//key: nodeId , value: measurment. every 30 seconds it will disconnect from the mesh, connect to WiFi, and push the last saved data
//start:
//#include <Arduino.h>
#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include "time.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <vector>
#include <map>
#include <cstdlib>
using namespace painlessmesh;
using namespace std;
/***************************
 *  Macro Definitions For the mesh
 **************************/
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
/***************************
 *  Variables Definitions For the WiFi - Change wifi ssid,password to match yours
 **************************/
const char* ssid     = "My_hotspot";
const char* password = "mypassword";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 7200;
/***************************
 *  Macro Definitions For the FireStore DB
 **************************/
#define API_KEY "AIzaSyCpnmHDV7B7oR6pnKXS0VBFoqaF174UzZM"
#define FIREBASE_PROJECT_ID "meshgarden-iot"
#define USER_EMAIL "ioadmin@ioadmin.com"
#define USER_PASSWORD "ioadmin"

class MeshBridge{

private:
    Scheduler userScheduler; // to control your personal task
    painlessMesh  mesh;
    //firebase global variables
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    //sync with the server, saving data variables
    int lasttime=0; //initialized, used to messure time interaval for the disconnect
    // std::map<String,vector<String>> dict;
    std::vector<String> server_data;

	friend void receivedCallback(uint32_t from, String &msg);
	friend void newConnectionCallback(uint32_t nodeId);
	friend void changedConnectionCallback();
	friend void nodeTimeAdjustedCallback(int32_t offset);

public:
void firebaseInit();
void init_node();
void update_node();
};