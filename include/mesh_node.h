#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include <painlessMesh.h>
#include <string.h>
#include <iostream>
#include <list>

// #include "sensor.cpp"
#include "soil_moisure_sensor_grove_v1.0.h"

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

class MeshNode
{
	private:
		painlessMesh mesh;
		Scheduler userScheduler; // to control your personal task
		// Task taskSendMessage; 
		// TODO should be list or set of tasks here
		Task measure;
		Task get_value;
		// std::list<Task> tasks_holder;
		int counter;
		std::list<int> counterList;

		friend void receivedCallback(uint32_t from, String &msg);
		friend void newConnectionCallback(uint32_t nodeId);
		friend void changedConnectionCallback();
		friend void nodeTimeAdjustedCallback(int32_t offset);
		// friend void sendMessage();

	public:
		MeshNode();
		void update();
		void sendMessage();
		void send_values(std::function<Measurement()> get_values_callback);
		void add_measurement(TaskCallback callable, unsigned long interval, long iterations=TASK_FOREVER);
		void add_send_values(std::function<Measurement()> callable, unsigned long interval, long iterations=TASK_FOREVER);
		void remove_task();
};


#endif /* _MESHNODE_H_ */