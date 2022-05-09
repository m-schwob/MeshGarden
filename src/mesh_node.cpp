#include "mesh_node.h"

static MeshNode *node = NULL;

// for Now the message simply contains the nodeId and a counter
void sendMessage()
{
	String msg;
	msg += node->mesh.getNodeId();
	msg += " ";
	msg += node->counter;
	node->counter++;
	Serial.println(msg);
	node->mesh.sendBroadcast(msg);
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


MeshNode::MeshNode() : taskSendMessage(TASK_SECOND * 2, TASK_FOREVER, &sendMessage), counter(0){
	Serial.println("Init node mesh connection..");
	// mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
	mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
	mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
	mesh.onReceive(&receivedCallback);
	mesh.onNewConnection(&newConnectionCallback);
	mesh.onChangedConnections(&changedConnectionCallback);
	mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
	mesh.onDroppedConnection([](uint32_t nodeId){ Serial.println(String(nodeId)); });
	userScheduler.addTask(taskSendMessage);
	taskSendMessage.enable();
	node = this;
}

void MeshNode::update(){
	mesh.update();
}

void MeshNode::add_task(TaskCallback callable){
	Task new_task(TASK_SECOND * 5, TASK_FOREVER, callable);
	userScheduler.addTask(new_task);
	new_task.enable();
}

void remove_task(){

}
