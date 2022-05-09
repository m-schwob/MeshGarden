/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/

  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
//*/
#include "painlessMesh.h"
#include "string.h"
#include <iostream>
#include <list>

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

class Mesh_Node
{
private:
	Scheduler userScheduler; // to control your personal task
	painlessMesh mesh;
	int counter;
	std::list<int> counterList;
	Task taskSendMessage;

	friend void receivedCallback(uint32_t from, String &msg);
	friend void newConnectionCallback(uint32_t nodeId);
	friend void changedConnectionCallback();
	friend void nodeTimeAdjustedCallback(int32_t offset);
	friend void sendMessage();

public:
	Mesh_Node() : counter(0), taskSendMessage(TASK_SECOND*20 , TASK_FOREVER, &sendMessage) {
		setup_node();
	}
	// User stub
	void mapNetwork();

	
	// Task mapAccessPoints( TASK_SECOND * 20, TASK_FOREVER , &mapNetwork);
	// TODO figgure out if there is need to split.
	void TaskManager()
	{
		userScheduler.addTask(taskSendMessage);
		taskSendMessage.enable();
	}
	

	void setup_node()
	{
		Serial.begin(115200);
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
    Serial.println(String(nodeId)); });
		TaskManager();
	}

	void loop_node()
	{
		// it will run the user scheduler as well
		mesh.update();
	}
};

// for Now the message simply contains the nodeId and a counter
void sendMessage()
{
	String msg;
	msg += mesh.getNodeId();
	msg += " ";
	msg += mesh.counter;
	counter++;
	Serial.println(msg);
	mesh.sendBroadcast(msg);
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
	Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

