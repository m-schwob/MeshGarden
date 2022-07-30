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
    Serial.println(AmPm);
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

        // int index = 0;
        vector<String> values = node->splitString(message["clock"].as<String>().c_str());
        Serial.println("clock message string:");
        Serial.println(message["clock"].as<String>());
        Serial.println("clock message end\n");

        node->setTimeVal((values[2]).c_str());
        node->date = values[0];
        node->AmPm = values[3];
        node->set_time = true;

        if (node->node_battery_level != -1)
        {
            DynamicJsonDocument battery_level(32);
            battery_level["battery"] = node->node_battery_level;
            Serial.println("created battery json");
            serializeJson(battery_level, Serial);
            node->mesh.sendSingle(from, battery_level.as<String>());
        }
    }

    if (message.containsKey("d"))
    {
        node->die_time.minutes = message["d"]["m"].as<int>();
        node->die_time.hours = message["d"]["h"].as<int>();
        node->die_time.seconds = message["d"]["s"].as<int>();
        node->die_interval = message["d"]["st"].as<int>();
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
    Serial.println("sends measures");
    node->emptyQueue();
}
void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback()
{
    Serial.printf("Changed connections at time: %d\n", node->mesh.getNodeTime());
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
    if(millis()-time_with_no_connections>=(sample_interval*1000) && !connected_to_bridge){
        Serial.println("entering deep sleep for no connection to bridge");
        store_timing(time, die_interval);
        ESP.deepSleep((sample_interval)*1000000);
    }
    if ((die_time.hours == time.hours || die_time.hours - 12 == time.hours) && die_time.minutes == time.minutes && die_time.seconds == time.seconds)
    {
        // put here store function
        Serial.println("entering deep sleep for timeout");
        store_timing(time, die_interval);
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

void MeshNode::store_timing(Time &time, int &sleep_time)
{
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(0, sleep_time);
    EEPROM.put(sizeof(sleep_time), time);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("time stored");
}

void MeshNode::load_timing(Time &time, int &sleep_time)
{
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(0, sleep_time);
    EEPROM.get(sizeof(sleep_time), time);
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

void MeshNode::setTimeVal(string str, string delimiter)
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
    printLocalTime();
}

void MeshNode::set_global_config(JsonObject global_config)
{
    // mesh settings
    MESH_PREFIX = global_config["mesh"]["mesh_prefix"].as<String>();
    MESH_PASSWORD = global_config["mesh"]["mesh_password"].as<String>();
    MESH_PORT = global_config["mesh"]["mesh_port"].as<size_t>();
    sample_interval = global_config["mesh"]["mesh_connection_time"].as<int>() + global_config["mesh"]["deep_sleep_time"].as<int>();
    Serial.println("config done, sample interval=" + String(sample_interval));
}

void MeshNode::init_clock()
{
    Time t;
    load_timing(t, die_interval);
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
    Serial.print(mesh.getNodeTime());
    Serial.println("node id: " + String(mesh.getNodeId()));
    node = this;
    alive = true;
}

void MeshNode::send_values(std::function<Measurements()> get_values_callback)
{
        Measurements meas;
        meas = get_values_callback();
        String time1;
        // printLocalTime();
        if (AmPm == "AM")
        {
            time1 += (time.hours < 10) ? "0" + String(time.hours - 3) + ":" : "0" + String(time.hours - 3) + ":";
            time1 += (time.minutes < 10) ? "0" + String(time.minutes) + ":" : String(time.minutes) + ":";
            time1 += (time.seconds < 10) ? "0" + String(time.seconds) : String(time.seconds);
        }
        else
        {
            time1 += String(time.hours + 12 - 3) + ":";
            time1 += (time.minutes < 10) ? "0" + String(time.minutes) + ":" : String(time.minutes) + ":";
            time1 += (time.seconds < 10) ? "0" + String(time.seconds) : String(time.seconds);
        }
        String timeStamp = date + "T" + time1 + "Z";
        DynamicJsonDocument measure1(256); // ameassure sample Json
        Serial.println("Sensor took " +String(meas.size()) + " measures");
        for (Measurement m : meas)
        {

            measure1["nodeId"] = mesh.getNodeId();
            measure1["sensorId"] = "sensor" + String(m.sensor_id);
            measure1["meassure_type"] = m.type;
            std::string m_type = std::string(m.type.c_str());
            std::replace(m_type.begin(), m_type.end(), ' ', '_');
            measure1["meassure_type"] = String(m_type.c_str());
            measure1["value"] = m.value;
            measure1["time"]["timestampValue"] = timeStamp;
            String mapKey = String(m.sensor_id)+"_"+String(m_type.c_str());
            if (measure1["meassure_type"].as<String>() != "")
            {
                String castString = measure1.as<String>();
                Serial.println("read message and set in queue:");
                Serial.println(measure1.as<String>());
                myqueue[mapKey].push(measure1.as<String>());
            }
        Serial.println("for key: " +String(mapKey) + " the queue contains: " + String(myqueue[mapKey].size()) );
    }
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

void MeshNode::call_measurements(){
    for(list<std::function<Measurements()>>::iterator iter = funcs.begin(); iter != funcs.end(); ++iter)
    {
        node->send_values(*iter);
    }
}

void MeshNode::emptyQueue(){
    for(std::map<String,queue<String>>::iterator iter = myqueue.begin(); iter != myqueue.end(); ++iter){
        while(!iter->second.empty() && mesh.isConnected(bridgeId)){
            mesh.sendSingle(bridgeId, iter->second.front());
            iter->second.pop();
        }
    }
        for(std::map<String,queue<String>>::iterator iter = myqueue.begin(); iter != myqueue.end(); ++iter){
            if(iter->second.empty())
                myqueue.erase(iter->first);
        }
}
