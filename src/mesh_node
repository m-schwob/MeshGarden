/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/
  
  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
//*/
#include "painlessMesh.h"
#include "string.h"
#include <iostream>
#include <list>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void mapNetwork();

//if the node is the wifi access point it will be true
int counter = 0;
int lasttime=0;
std::list<int> counterList;

Task taskSendMessage( TASK_SECOND * 20 , TASK_FOREVER, &sendMessage );
//Task mapAccessPoints( TASK_SECOND * 20, TASK_FOREVER , &mapNetwork);
//TODO figgure out if there is need to split.
void TaskManager(){
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
//for Now the message simply contains the nodeId and a counter 
void sendMessage() {
  String msg;
  msg+= mesh.getNodeId();
  msg +=" ";
  msg +=counter;
  counter++;
  Serial.println(msg);
  mesh.sendBroadcast(msg);
  }

// void mapNetwork(){
//   std::list<uint32_t> nodes = mesh.getNodeList();
//   std::list<uint32_t>::iterator it;
//   Serial.printf("gotHere list size is: %d\n",nodes.size());
//   for (it = nodes.begin(); it != nodes.end(); ++it)
//     Serial.printf("node: %u\n",*it); 
// }
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onDroppedConnection([](uint32_t nodeId) {
   // Do something with the event
   Serial.println(String(nodeId));
});
  TaskManager();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();

}