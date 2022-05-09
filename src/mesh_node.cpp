#include "mesh_node.h"

static Mesh_Node *node = NULL;

// for Now the message simply contains the nodeId and a counter
void sendMessage()
{
	// String msg;
	// msg += mesh.getNodeId();
	// msg += " ";
	// msg += counter;
	// counter++;
	// Serial.println(msg);
	// mesh.sendBroadcast(msg);
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


Mesh_Node::Mesh_Node() : taskSendMessage(TASK_SECOND * 20, TASK_FOREVER, &sendMessage), counter(0){
	Serial.begin(115200);
	// mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
	mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
	mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
	mesh.onReceive(&receivedCallback);
	mesh.onNewConnection(&newConnectionCallback);
	mesh.onChangedConnections(&changedConnectionCallback);
	mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
	mesh.onDroppedConnection([](uint32_t nodeId){ Serial.println(String(nodeId)); });
	taskSendMessage.enable();
	node = this;
}

void Mesh_Node::update(){
	mesh.update();
}
