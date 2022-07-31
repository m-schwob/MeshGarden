#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include <painlessMesh.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include "time.h"
#include "EEPROM.h"

#include "constants_utils.h"
#include "sensor.h"
// #include "mesh_garden.h"

using namespace std;

// #define MESH_PREFIX "whateverYouLike"
// #define MESH_PASSWORD "somethingSneaky"
// #define MESH_PORT 5555
class MeshNode
{
private:
	Scheduler userScheduler; // to control your personal task
	// Task taskSendMessage;
	// TODO should be list or set of tasks here
	// Task measure;
	Task get_value;
	std::list<Task> measure;
	int counter = 0;
	bool alive = false;
	bool set_time = false;
	std::list<int> counterList;
	int timer;

	String MESH_PREFIX;
	String MESH_PASSWORD;
	unsigned int MESH_PORT;

	friend void receivedCallback(uint32_t from, String &msg);
	friend void newConnectionCallback(uint32_t nodeId);
	friend void changedConnectionCallback();
	friend void nodeTimeAdjustedCallback(int32_t offset);
	// friend void sendMessage();

public:
	painlessMesh mesh;
	int lasttime = 0;
	unsigned long time_with_no_connections = 0;
	bool connected_to_bridge = false;
	bool initialized = false;
	bool configure_ready = false;
	String config_string;
	MeshNode();
	void update();
	void sendMessage();
	void remove_task();
	vector<String> splitString(string str, string delimiter = " ");
	list<std::function<Measurements()>> funcs;
	void setTimeVal(string str,String AmPm, string delimiter = ":");
	void set_global_config(JsonObject global_config);
	void init_mesh();
	void init_clock();

	// time variables:
	unsigned long timeNow = 0;
	unsigned long timeLast = 0;
	int startingHour = 12; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time

	Time time;
	int days = 0;
	String date;

	// Accuracy settings
	int dailyErrorFast = 0;	  // set the average number of milliseconds your microcontroller's time is fast on a daily basis
	int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis
	int correctedToday = 1;	  // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.
	void time_update();
	void store_timing(Time &time, int &sleep_time,int& lost_connection_interval_counter);
	void load_timing(Time &time, int &sleep_time,int& lost_connection_interval_counter);
	void printLocalTime();
	uint32_t bridgeId = 0;
	bool sent_in_the_interval = false;
	// measurment settings:
	void add_measurement(std::function<Measurements()> callable, unsigned long interval, long iterations);
	void send_values(std::function<Measurements()> get_values_callback);
	Time reset_time;
	Time die_time;
	int die_interval = 10000;
	queue<Measurement> myqueue;
	Time calculate_time();

	void get_battery_level(Measurement battery_level);
	void call_measurements();
	void emptyQueue();
	float node_battery_level = -1;
	void calculate_death();

	int wake_up_time;
	int deep_sleep_time;
	int lost_connection_interval_counter=0;
};

#endif /* _MESHNODE_H_ */

/*past code:


*/