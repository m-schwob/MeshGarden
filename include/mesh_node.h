#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include <painlessMesh.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>
#include <iostream>
#include "time.h"
#include "sensor.h"
// #include "mesh_garden.h"

using namespace std;
#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

class MeshNode
{
	private:
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
		painlessMesh mesh;
		int lasttime = 0;
		bool dead = false;
		bool initialized = false;
		bool configure_ready = false;
		String config_string;
		MeshNode();
		void update();
		void sendMessage();
		void remove_task();
		vector<String> splitString(string str, string delimiter = " ");
		void setTimeVal(string str, string delimiter = ":");
		void init_mesh();
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
	uint32_t bridgeId = 0 ;
	//measurment settings:
	void add_measurement(std::function<Measurements()> callable, unsigned long interval, long iterations);
	void send_values(std::function<Measurements()> get_values_callback);

	int die_minute=0;
	int die_hour=25;
	int die_second = 0;
	int die_time=1000;
};


#endif /* _MESHNODE_H_ */

/*past code:


*/