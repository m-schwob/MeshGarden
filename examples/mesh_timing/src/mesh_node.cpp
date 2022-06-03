#include "mesh_node.h"


static MeshNode *node = NULL;

// for Now the message simply contains the nodeId and a counter
void MeshNode::sendMessage()
{
    String msg;
    msg += mesh.getNodeId();
    msg += " ";
    msg += counter;
    counter++;
    Serial.println(msg);
    mesh.sendBroadcast(msg);
}

void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
    if(msg == "die"){
        Serial.println("stop mesh");
        node->mesh.stop();
    }
}

void newConnectionCallback(uint32_t nodeId)
{
    uint32_t time1= node->mesh.getNodeTime();
    Serial.printf("--> startHere: New Connection, nodeId = %u at time %d\n", nodeId, time1);

}

void changedConnectionCallback()
{
    Serial.printf("Changed connections at time: %d\n", node->mesh.getNodeTime());
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", node->mesh.getNodeTime(), offset);
    node->mesh.sendBroadcast(" node Adjusted time: %u\n" ,node->mesh.getNodeTime());

}

MeshNode::MeshNode() : counter(0) //, taskSendMessage(TASK_SECOND * 2, TASK_FOREVER, [this](){sendMessage();})
{
    timer= millis();
    Serial.printf("Init node mesh connection:%d\n",timer);
    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onDroppedConnection([](uint32_t nodeId)
                             {
        Serial.printf("node dropped:%u, at time: %u",nodeId,node->mesh.getNodeTime());}
                            );
    Task taskSendMessage(TASK_SECOND * 2, TASK_FOREVER, [this](){sendMessage();});
    userScheduler.addTask(taskSendMessage);
    taskSendMessage.enable();
    Serial.print(mesh.getNodeTime());
    int timer2 = millis();
    Serial.printf("after mesh init difference: %d\n",timer2-timer);
    timer=timer2;
    node = this;
}
void MeshNode::update()
{
    mesh.update();
}
