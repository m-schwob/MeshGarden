#include "mesh_node.h"
static MeshNode *node = NULL;

void MeshNode::printLocalTime(){
Serial.print(date);
Serial.print(" and The time is:           ");
Serial.print(days);
Serial.print(":");
Serial.print(hours);
Serial.print(":");
Serial.print(minutes);
Serial.print(":"); 
Serial.print(seconds); 
Serial.print(" ");
Serial.println(AmPm);
}

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
    // if(msg == "die"){
    //     Serial.println("stop mesh");
    //     node->mesh.stop();
    //     node->alive = false;
    //     return;
    // }
    DynamicJsonDocument message(msg.length()+64);
    DeserializationError error = deserializeJson(message, msg);
    Serial.println("\nJSON DESERIALIZED");
    Serial.println(message.as<String>());
    Serial.println("JSON DESERIALIZED end\n\n");
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    }

    if(message.containsKey("clock")){
        Serial.println(msg);
        Serial.println("message:");
        Serial.println(message["clock"].as<String>());

        int index = 0;
        vector<String> values = node->splitString(message["clock"].as<String>().c_str());
        Serial.println("clock message string:");
        Serial.println(message["clock"].as<String>());
        Serial.println("clock message end\n");
        
        for(vector<String>::iterator it = values.begin(); it != values.end(); ++it ) {
            Serial.printf("at index: %d\n",index);
            Serial.println(*it); // prints d.
            index++;
        }

        node->setTimeVal((values[3]).c_str());
        node->date = values[0] + " " + values[1] +" "+values[2];
        node->AmPm = values[4];
        node->set_time= true;
    }
    if(message.containsKey("change")){
        //new configurations
        Serial.println("\nread confugires:");
        node->config_string = message["change"].as<String>();
        // node->configure_ready=false;
        Serial.println(message["change"].as<String>());
        Serial.println("end read confugires:\n");
        node->configure_ready=true;
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
    Serial.begin(115200);
    timer= millis();
    Serial.printf("Init node mesh connection:%d\n",timer);
    mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    //mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
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
    alive = true;
}

void MeshNode::update()
{   
    time_update();
    if(!alive){
        ESP.deepSleep(3e6);
        init_mesh();
        alive = true;
    }
    if(millis()-counter > 10000 && set_time){
        Serial.print("got here");
        printLocalTime();
        counter = millis();
    }
    if(alive)
        mesh.update();
}

void MeshNode::time_update(){
    
    timeNow = millis()/1000; // the number of milliseconds that have passed since boot
    seconds = timeNow - timeLast;//the number of seconds that have passed since the last time 60 seconds was reached.

    if (seconds == 60) {
        timeLast = timeNow;
        minutes = minutes + 1;
    }

//if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

    if (minutes == 60){ 
        minutes = 0;
        hours = hours + 1;
    }

// if one hour has passed, start counting minutes from zero and add one hour to the clock

    if (hours == 24){
        hours = 0;
        days = days + 1;
        }

  //if 24 hours have passed , add one day

    if (hours ==(24 - startingHour) && correctedToday == 0){
        delay(dailyErrorFast*1000);
        seconds = seconds + dailyErrorBehind;
        correctedToday = 1;
    }

//every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds. 
//Change these varialbes accordin\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\g to the error of your board. 
// The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days 
// and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

    if (hours == 24 - startingHour + 2) { 
    correctedToday = 0;
    }

//let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono
// would continue to correct for an entire hour that is 24 - startingHour. 

}

vector<String> MeshNode::splitString(string str, string delimiter)
{
    vector<String> ret;
    int start = 0;
    int end = str.find(delimiter);
    while (end != -1) {
        ret.push_back(String((str.substr(start, end - start).c_str())));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    ret.push_back(String((str.substr(start, end -2 - start)).c_str()));
    return ret;
}

void MeshNode::setTimeVal(string str, string delimiter)
{
    Serial.println("setTimeVal function got:");
    vector<int> ret;
    int start = 0;
    int end = str.find(delimiter);
    int index = 0;
    while (end != -1) {
        Serial.printf("to index %d , we put: %s\n",index,str.substr(start, end - start).c_str());
        ret.push_back(atoi(str.substr(start, end - start).c_str()));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
        index++;
    }
    ret.push_back(atoi(str.substr(start, end - start).c_str()));
    Serial.printf("to index %d , we put: %s",index,str.substr(start, end - start).c_str());
    startingHour = ret[0];
    hours= ret[0];
    minutes= ret[1];
    seconds = ret[2];
}

void MeshNode:: init_mesh(){

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
        Serial.println("node id:");
        Serial.println(mesh.getNodeId());
        timer=timer2;
        node = this;
        alive = true;
    }

void MeshNode::send_values(std::function<Measurement()> get_values_callback){
    Serial.println("sending values");
    //add it when we will have a sensor
    Measurement meas;
    do {
        meas = get_values_callback();
        mesh.sendBroadcast(mesh.getNodeId() + "," + meas.type + "," + meas.value);
    } while(!meas.last);
    mesh.sendBroadcast(mesh.getNodeId() + ",soil measure , 10" );
}

void MeshNode::add_measurement(TaskCallback callable, unsigned long interval, long iterations)
{
Serial.println("adding measurement task");
measure.set(TASK_SECOND*interval, iterations, callable);
userScheduler.addTask(measure);
measure.enable();
}

