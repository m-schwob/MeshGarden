#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include <painlessMesh.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>
#include <iostream>
#include "time.h"

using namespace std;
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
		int counter = 0;
		bool alive = false;
		bool set_time = false;
		std::list<int> counterList;
		int timer;
		friend void receivedCallback(uint32_t from, String &msg);
		friend void newConnectionCallback(uint32_t nodeId);
		friend void changedConnectionCallback();
		friend void nodeTimeAdjustedCallback(int32_t offset);
		// friend void sendMessage();

	public:
		int lasttime = 0;
		bool dead = false;
		MeshNode();
		void update();
		void sendMessage();
		void add_measurement(TaskCallback callable, unsigned long interval, long iterations=TASK_FOREVER);
		void remove_task();
		vector<String> splitString(string str, string delimiter = " ");
		void setTimeVal(string str, string delimiter = ":");
	//time variables:
	unsigned long timeNow = 0;
	unsigned long timeLast = 0;
	int startingHour = 12; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
	int seconds = 0;
	int minutes = 0;
	int hours = startingHour;
	int days = 0;
	String date;
	String AmPm;
	//Accuracy settings
	int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
	int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis
	int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.  
	void time_update();
	void printLocalTime();
    void init_mesh();
};


#endif /* _MESHNODE_H_ */