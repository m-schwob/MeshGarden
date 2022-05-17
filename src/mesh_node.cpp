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

void MeshNode::send_values(std::function<Measurement()> get_values_callback){
    Serial.println("sending values");
    //add it when we will have a sensor
    // Measurement meas;
    // do {
    //     meas = get_values_callback();
    //     mesh.sendBroadcast(mesh.getNodeId() + "," + meas.type + "," + meas.value);
    // } while(!meas.last);
    mesh.sendBroadcast(mesh.getNodeId() + ",soil measure , 10" );
    }

void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", node->mesh.getNodeTime(), offset);
}

MeshNode::MeshNode() : counter(0) //, taskSendMessage(TASK_SECOND * 2, TASK_FOREVER, [this](){sendMessage();})
{
    Serial.println("Init node mesh connection..");
    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onDroppedConnection([](uint32_t nodeId)
                             { Serial.println(String(nodeId)); });
    Task taskSendMessage(TASK_SECOND * 2, TASK_FOREVER, [this](){sendMessage();});
    userScheduler.addTask(taskSendMessage);
    taskSendMessage.enable();
    node = this;
}

void MeshNode::update()
{
    mesh.update();
}

void MeshNode::add_measurement(TaskCallback callable, unsigned long interval, long iterations)
{
    // Serial.println("adding measurement task");
    // measure.set(TASK_SECOND*interval, iterations, callable);
    // userScheduler.addTask(measure);
    // measure.enable();
}

void MeshNode::add_send_values(std::function<Measurement()> callable, unsigned long interval, long iterations)
{
    // Serial.println("adding value reading task");
    // get_value.set(TASK_SECOND*interval, iterations, [this, callable](){send_values(callable);});
    // userScheduler.addTask(get_value);
    // get_value.enable();
}

void remove_task()
{
}


