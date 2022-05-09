#include <painlessMesh.h>
#include <string.h>
#include <iostream>
#include <list>

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

class Mesh_Node
{
	private:
		painlessMesh mesh;
		Scheduler userScheduler; // to control your personal task
		Task taskSendMessage;
		int counter;
		std::list<int> counterList;

		friend void receivedCallback(uint32_t from, String &msg);
		friend void newConnectionCallback(uint32_t nodeId);
		friend void changedConnectionCallback();
		friend void nodeTimeAdjustedCallback(int32_t offset);
		friend void sendMessage();

	public:
		Mesh_Node();
		void update();
};
