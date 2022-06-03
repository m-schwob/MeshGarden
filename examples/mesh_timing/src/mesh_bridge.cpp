#include "mesh_bridge.h"


static MeshBridge *node = NULL;

MeshBridge::MeshBridge(){
    init_mesh();
    node = this;
}

// Needed for painless library
// receivedCallback - when we get a post on the mesh, it will update the dict by the message sent to it
// the dict will have nodeId as its key, and the Entire message as its value
void receivedCallback(uint32_t from, String &msg)
{
    uint32_t timer = node->mesh.getNodeTime();
    Serial.printf("message received from %u msg=%s. at time: %u\n", from, msg.c_str(),timer);
    // node->server_data.push_back(String(from) + "," + msg);
}

// Needed for mesh, do not change
void newConnectionCallback(uint32_t nodeId)
{
    uint32_t time = node->mesh.getNodeTime();
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    Serial.printf("new node connected at time: %u\n", time);
}

void changedConnectionCallback()
{  
    Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", node->mesh.getNodeTime(), offset);
}

void MeshBridge::init_mesh()
{   
     myTime = millis();
    // Serial.printf("Mesh node start at time: %d \n", myTime);
    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onDroppedConnection([](uint32_t nodeId)
    {
   // Do something with the event
   Serial.print("changed connectio with node" + String(nodeId)+ "at time: "+ node->mesh.getNodeTime());
    });
   mesh.onNodeDelayReceived([](uint32_t nodeId, int delay) {
       // Do something with the event
       Serial.println(String(delay));
    });
    // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
    mesh.setRoot(true);
    // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
    mesh.setContainsRoot(true);
}

void MeshBridge::update()
{
    // it will run the user scheduler as well
    if(millis()-myTime >5000)
    {
    // Serial.print(mesh.getNodeTime());
    // std::list<uint32_t> nodesList = mesh.getNodeList();
    // for (std::list<uint32_t>::iterator it = nodesList.begin(); it != nodesList.end(); it++){
    // mesh.sendSingle(*it,"die");
    // }
    // delay(1000);
    Serial.print(F("FreeHeap "));
    Serial.println(ESP.getFreeHeap());
    mesh.sendBroadcast("stop mesh");
    mesh.stop();
    // Serial.printf("disconnect");  
    delay(20000);
    init_mesh();
    myTime=millis();
    }
    else{
    mesh.update();
    }
    // Init and get the time
    // if (millis() - lasttime >10000)
    // {
    //     // Serial.printf("disconnect time: %d", mesh.getNodeTime());
    //     // mesh.stop();
    //     // int myTime2 = millis();
    //     // Serial.printf("Mesh node reset at time: %d \n", myTime2);
    //     // // Connect to Wi-Fi
    //     // init_mesh();
    //     // int myTime3 =millis();
    //     // Serial.printf("Mesh node setUp at time: %d\n", myTime3);
    //     // Serial.printf("Mesh node setUp revive diff: %d\n", myTime3-myTime2);
    //     // myTime2=myTime3;
    //     lasttime = millis();
    // }
}
