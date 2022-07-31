#include "mesh_node.h"
static MeshNode *node = NULL;

void MeshNode::printLocalTime()
{
    Serial.print(date);
    Serial.print(" and The time is: ");
    Serial.print(days);
    Serial.print(":");
    Serial.print(time.hours);
    Serial.print(":");
    Serial.print(time.minutes);
    Serial.print(":");
    Serial.print(time.seconds);
    Serial.print(" ");
    if(time.Am)
        Serial.print("AM\n");
    else
        Serial.print("PM\n");
}

/*
input: 2 time structs , int value of the number of minutes between them
output: true - if between the two time frames there were less then those minutes
false- otherwise
*/
bool computeTimeDifference(Time t1, Time t2, int acceptable_difference_minutes){
    
    Time difftime;
    if(t2.seconds > t1.seconds)
    {
        --t1.minutes;
        t1.seconds += 60;
    }

    difftime.seconds= t1.seconds - t2.seconds;
    if(t2.minutes > t1.minutes)
    {
        --t1.hours;
        t1.minutes += 60;
    }
    difftime.minutes= t1.minutes-t2.minutes;
    difftime.hours= t1.hours-t2.hours;
    if (difftime.hours ==0 && difftime.minutes<=acceptable_difference_minutes)
        return true;
    return false;
}
/*
must function for the painlessmesh network.
input:  uint32_t from - the node id of the node from whom we got the message (always will be the bridge id)
        String msg containing the message passed, the message is always in JSON format
output: NONE
the function will act differentelly according to the message it had recive.
if got a "clock" message- the node will set its time values and will start counting the time
if got a "d" message - the node will set its next time of death
if got a "change" message - that mean the user changed the configurations of the node. it will set the new configs and reset the node.
*/
void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
    node->connected_to_bridge=true;
    node->lost_connection_interval_counter=0;
    DynamicJsonDocument message(msg.length() + 64);
    DeserializationError error = deserializeJson(message, msg);
    Serial.println("\nJSON DESERIALIZED");
    Serial.println(message.as<String>());
    Serial.println("JSON DESERIALIZED end\n\n");
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    }

    if (message.containsKey("clock"))
    {
        node->bridgeId = from;
        Serial.println(message["clock"].as<String>());
        node->reset_time=node->time;
        // int index = 0;
        vector<String> values = node->splitString(message["clock"].as<String>().c_str());
        Serial.println("clock message string:");
        Serial.println(message["clock"].as<String>());
        Serial.println("clock message end\n");

        node->setTimeVal((values[2]).c_str(), values[3]);
        node->date = values[0];
        node->set_time = true;

        if (node->node_battery_level != -1)
        {
            DynamicJsonDocument battery_level(32);
            battery_level["battery"] = node->node_battery_level;
            Serial.println("created battery json");
            serializeJson(battery_level, Serial);
            Serial.println("sending battery signal");
            node->mesh.sendSingle(from, battery_level.as<String>());
        }
    }

    if (message.containsKey("d"))
    {
    int r_sec = message["d"]["s"].as<int>();
    int r_min = message["d"]["m"].as<int>();
    int r_hour= message["d"]["h"].as<int>();
    node->die_time.seconds = (r_sec + 1) % 60;
    node->die_time.minutes = ((r_min + ((r_sec + 1) / 60)) % 60);
    node->die_time.hours = ((r_hour + ((r_min + ((r_sec + 1) / 60)) / 60)));
    node->die_interval = message["d"]["st"].as<int>();
    Serial.println(String(node->die_time.hours) + ":" + String(node->die_time.minutes) + ":" + String(node->die_time.seconds));
    }
    if (message.containsKey("change"))
    {
        // new configurations
        Serial.println("\nread confugires:");
        node->config_string = message["change"].as<String>();
        // node->configure_ready=false;
        Serial.println(message["change"].as<String>());
        Serial.println("end read confugires:\n");
        node->configure_ready = true;
    }
    if(!node->sent_in_the_interval && node->bridgeId!=0 && node->mesh.isConnected(node->bridgeId)){    
    if(!node->myqueue.empty() && computeTimeDifference(node->time , node->reset_time, 5)){
        node->emptyQueue();
        node->sent_in_the_interval=true;
    }
    else{
        Serial.println("taking measures again");
        node->call_measurements();
        node->emptyQueue();
        node->sent_in_the_interval=true;
    }
    }
}
void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback()
{
    // Serial.printf("Changed connections at time: %d\n", node->mesh.getNodeTime());
    if(!node->sent_in_the_interval && node->bridgeId!=0 && node->mesh.isConnected(node->bridgeId)){    
    if(!node->myqueue.empty() &&computeTimeDifference(node->time , node->reset_time, 5)){
        node->emptyQueue();
        node->sent_in_the_interval=true;
    }
    else{
        node->call_measurements();
        node->emptyQueue();
        node->sent_in_the_interval=true;
    }
    }
}
void nodeTimeAdjustedCallback(int32_t offset)
{
    // Serial.printf("Adjusted time %u. Offset = %d\n", node->mesh.getNodeTime(), offset);
}

MeshNode::MeshNode() : counter(0) {}

void MeshNode::update()
{
    if (millis() - lasttime >= 1000)
    {
        time_update();
        lasttime = millis();
    }
    if(millis() - time_with_no_connections>=(wake_up_time*1000) && !connected_to_bridge){

        Serial.println("entering deep sleep for no connection to bridge + was up for:" + String(millis() - time_with_no_connections)+ " lcic " + String(lost_connection_interval_counter));
        lost_connection_interval_counter+=1;
        Serial.println("Storing time:" + String(time.hours)+":"+String(time.minutes)+":"+String(time.seconds));

        store_timing(time, die_interval, lost_connection_interval_counter);
        ESP.deepSleep((deep_sleep_time)*1000000);
    }
    if ((die_time.hours == time.hours || die_time.hours - 12 == time.hours) && die_time.minutes == time.minutes && die_time.seconds == time.seconds)
    {
        // put here store function
        Serial.println("entering deep sleep for timeout for " + String(die_interval) + " lcic " + String(lost_connection_interval_counter));
        Serial.println("Storing time:" + String(time.hours)+":"+String(time.minutes)+":"+String(time.seconds));
        store_timing(time, die_interval,lost_connection_interval_counter);
        ESP.deepSleep((die_interval)*1000000);
    }
    if (alive)
        mesh.update();
}

void MeshNode::time_update()
{
    // timeNow = millis()/1000; // the number of milliseconds that have passed since boot
    time.seconds += 1; // the number of seconds that have passed since the last time 60 seconds was reached.

    if (time.seconds == 60)
    {
        timeLast = timeNow;
        time.minutes = time.minutes + 1;
        time.seconds = 0;
    }
    // if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

    if (time.minutes == 60)
    {
        time.minutes = 0;
        time.hours = time.hours + 1;
    }

    // if one hour has passed, start counting minutes from zero and add one hour to the clock

    if (time.hours == 24)
    {
        time.hours = 0;
        days = days + 1;
    }

    // if 24 hours have passed , add one day

    if (time.hours == (24 - startingHour) && correctedToday == 0)
    {
        delay(dailyErrorFast * 1000);
        time.seconds = time.seconds + dailyErrorBehind;
        correctedToday = 1;
    }

    // every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds.
    // Change these varialbes accordin\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\g to the error of your board.
    //  The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days
    //  and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

    if (time.hours == 24 - startingHour + 2)
    {
        correctedToday = 0;
    }

    // let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono
    //  would continue to correct for an entire hour that is 24 - startingHour.
}

void MeshNode::store_timing(Time &time, int &sleep_time, int& lost_connection_interval_counter)
{
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(0, sleep_time);
    EEPROM.put(sizeof(sleep_time), time);
    EEPROM.put(sizeof(time),lost_connection_interval_counter);
    //int size_of_queue = (sizeof(Measurement)*myqueue.size())*2;
    //EEPROM.put(size_of_queue,myqueue);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("time stored");
}

void MeshNode::load_timing(Time &time, int &sleep_time, int& lost_connection_interval_counter)
{
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(0, sleep_time);
    EEPROM.get(sizeof(sleep_time), time);
    EEPROM.get(sizeof(time),lost_connection_interval_counter);
    EEPROM.end();
    Serial.println("time loaded");
}

vector<String> MeshNode::splitString(string str, string delimiter)
{
    vector<String> ret;
    int start = 0;
    int end = str.find(delimiter);
    while (end != -1)
    {
        ret.push_back(String((str.substr(start, end - start).c_str())));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    ret.push_back(String((str.substr(start, end - 2 - start)).c_str()));
    return ret;
}

void MeshNode::setTimeVal(string str, String AmPm,string delimiter)
{
    Serial.println("set Time:");
    vector<int> ret;
    int start = 0;
    int end = str.find(delimiter);
    int index = 0;
    while (end != -1)
    {
        ret.push_back(atoi(str.substr(start, end - start).c_str()));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
        index++;
    }
    ret.push_back(atoi(str.substr(start, end - start).c_str()));
    Serial.printf("at 0: %d , at 1: %d , at 2 %d\n", ret[0], ret[1], ret[2]);
    startingHour = ret[0];
    time.hours = ret[0];
    time.minutes = ret[1];
    time.seconds = ret[2];
    Serial.printf("hours: %d , minutes: %d , seconds %d\n", time.hours, time.minutes, time.seconds);
    if(AmPm=="AM")
        time.Am=1;
    else
        time.Am=0;
    printLocalTime();
}

void MeshNode::set_global_config(JsonObject global_config)
{
    // mesh settings
    MESH_PREFIX = global_config["mesh"]["mesh_prefix"].as<String>();
    MESH_PASSWORD = global_config["mesh"]["mesh_password"].as<String>();
    MESH_PORT = global_config["mesh"]["mesh_port"].as<size_t>();
    wake_up_time =  global_config["mesh"]["mesh_connection_time"].as<int>();
    deep_sleep_time = global_config["mesh"]["deep_sleep_time"].as<int>();
}

void MeshNode::init_clock()
{
    Time t;
    load_timing(t, die_interval,lost_connection_interval_counter);
    time.seconds = (t.seconds + die_interval) % 60;
    time.minutes = ((t.minutes + ((t.seconds + die_interval) / 60)) % 60);
    time.hours = ((t.hours + ((t.minutes + ((t.seconds + die_interval) / 60)) / 60)) % 24);
    time = t;
    printLocalTime();
}

void MeshNode::init_mesh()
{
    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onDroppedConnection([](uint32_t nodeId)
                             { Serial.printf("node dropped:%u, at time: %u", nodeId, node->mesh.getNodeTime()); });
    Serial.println("node id: " + String(mesh.getNodeId()));
    node = this;
    alive = true;
    if(lost_connection_interval_counter >=3){
        deep_sleep_time =deep_sleep_time+wake_up_time; 
        wake_up_time=deep_sleep_time;
    }
}

void MeshNode::send_values(std::function<Measurements()> get_values_callback)
{
        Measurements meas;
        meas = get_values_callback();

    for (Measurement m : meas)
    {            
        if (m.type != "")
        {
            String mapKey = String(m.sensor_id)+"_"+String(m.type.c_str());
            m.time= time;
            Serial.println("read message and set in queue:");
            Serial.println(String(m.sensor_id) + " " + String(m.type));
            myqueue.push(m);
        }
    }
    Serial.println("queue size is:" + String(myqueue.size()));
}

void MeshNode::add_measurement(std::function<Measurements()> callable, unsigned long interval, long iterations)
{
    Serial.println("adding measurement task");
    funcs.push_back(callable);
}

void MeshNode::get_battery_level(Measurement battery_level)
{
    Serial.println("set bettery level to: " + String(battery_level.value));
    node_battery_level = battery_level.value;
}

void firestoreMapBatteryUpdate(String nodeId, float value) {}


void MeshNode::call_measurements(){
    for(list<std::function<Measurements()>>::iterator iter = funcs.begin(); iter != funcs.end(); ++iter)
    {
        node->send_values(*iter);
    }
}

void MeshNode::emptyQueue(){
    Serial.println("number of measurements:" + String(myqueue.size()));
        while(!myqueue.empty() && mesh.isConnected(bridgeId)){
            Measurement meas = myqueue.front();
            String time1;
            Serial.println("sending values we are at: " + time.Am);
            Serial.println("meas time:" + String(meas.time.hours) + ":" + String(meas.time.minutes)+ ":" + String(meas.time.seconds));
            if (time.Am == 1)
            {
                if(((meas.time.hours +24 - 3)%12) > 10)
                    time1 += String((meas.time.hours +24 - 3)%12)+ ":";
                else
                    time1 += "0" + String((meas.time.hours +24 - 3)%12)+ ":";
                time1 += (meas.time.minutes < 10) ? "0" + String(meas.time.minutes) + ":" : String(meas.time.minutes) + ":";
                time1 += (meas.time.seconds < 10) ? "0" + String(meas.time.seconds) : String(meas.time.seconds);
            }
            else
            {
                time1 += String(meas.time.hours + 12 - 3) + ":";
                time1 += (meas.time.minutes < 10) ? "0" + String(meas.time.minutes) + ":" : String(meas.time.minutes) + ":";
                time1 += (meas.time.seconds < 10) ? "0" + String(meas.time.seconds) : String(meas.time.seconds);
            }

            String timeStamp = date + "T" + time1 + "Z";
            DynamicJsonDocument measure1(256); // ameassure sample Json
            measure1["nodeId"] = mesh.getNodeId();
            measure1["sensorId"] = "sensor" + String(meas.sensor_id);
            std::string m_type = std::string(meas.type.c_str());
            std::replace(m_type.begin(), m_type.end(), ' ', '_');
            measure1["meassure_type"] = String(m_type.c_str());
            measure1["value"] = meas.value;
            measure1["time"]["timestampValue"] = timeStamp;
            Serial.println("sending measurement");
            mesh.sendSingle(bridgeId, measure1.as<String>());
            myqueue.pop();
        }
}

