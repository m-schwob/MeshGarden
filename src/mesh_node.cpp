#include "mesh_node.h"
static MeshNode *node = NULL;

void MeshNode::printLocalTime()
{
    Serial.print(date);
    Serial.print(" and The time is: ");
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

void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());

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
    }

    if (message.containsKey("d"))
    {
        node->die_minute = message["d"]["m"].as<int>();
        node->die_hour = message["d"]["h"].as<int>();
        node->die_second = message["d"]["s"].as<int>();
        node->die_time = message["d"]["st"].as<int>();
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
    Serial.printf("Adjusted time %u. Offset = %d\n", node->mesh.getNodeTime(), offset);
}

MeshNode::MeshNode() : counter(0) //, taskSendMessage(TASK_SECOND * 2, TASK_FOREVER, [this](){sendMessage();})
{
}

void MeshNode::update()
{
    if (millis() - lasttime >= 1000)
    {
        time_update();
        printLocalTime();
        Serial.printf("%d:%d:%d\n", die_hour, die_minute, die_second);
        lasttime = millis();
    }
    if ((die_hour == hours || die_hour - 12 == hours) && die_minute == minutes && die_second == seconds)
    {
        // put here store function
        Serial.println("die");
        ESP.deepSleep((die_time)*1000000);
    }
    if (alive)
        mesh.update();
}

void MeshNode::time_update()
{
    // timeNow = millis()/1000; // the number of milliseconds that have passed since boot
    seconds += 1; // the number of seconds that have passed since the last time 60 seconds was reached.

    if (seconds == 60)
    {
        timeLast = timeNow;
        minutes = minutes + 1;
        seconds = 0;
    }
    // if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

    if (minutes == 60)
    {
        minutes = 0;
        hours = hours + 1;
    }

    // if one hour has passed, start counting minutes from zero and add one hour to the clock

    if (hours == 24)
    {
        hours = 0;
        days = days + 1;
    }

    // if 24 hours have passed , add one day

    if (hours == (24 - startingHour) && correctedToday == 0)
    {
        delay(dailyErrorFast * 1000);
        seconds = seconds + dailyErrorBehind;
        correctedToday = 1;
    }

    // every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds.
    // Change these varialbes accordin\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\g to the error of your board.
    //  The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days
    //  and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

    if (hours == 24 - startingHour + 2)
    {
        correctedToday = 0;
    }

    // let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono
    //  would continue to correct for an entire hour that is 24 - startingHour.
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
    hours = ret[0];
    minutes = ret[1];
    seconds = ret[2];
    Serial.printf("hours: %d , minutes: %d , seconds %d\n", hours, minutes, seconds);
    printLocalTime();
}

void MeshNode::set_global_config(JsonObject global_config)
{
    // mesh settings
    MESH_PREFIX = global_config["mesh"]["mesh_prefix"].as<String>();
    MESH_PASSWORD = global_config["mesh"]["mesh_password"].as<String>();
    MESH_PORT = global_config["mesh"]["mesh_port"].as<size_t>();
    Serial.println("config done");
}

void MeshNode::init_clock() {}

void MeshNode::listenQueue()
{
    if (mesh.isConnected(bridgeId) && !myqueue.empty())
    {
        mesh.sendSingle(bridgeId, myqueue.front());
        myqueue.pop();
    }
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
    // Task update_time(TASK_SECOND * 1, TASK_FOREVER, [this](){time_update();});
    Task emptyQueue(TASK_SECOND * 1.5, TASK_FOREVER, [this]()
                    { listenQueue(); });

    // userScheduler.addTask(update_time);
    // update_time.enable();
    userScheduler.addTask(emptyQueue);
    emptyQueue.enable();

    Serial.print(mesh.getNodeTime());
    Serial.println("node id: " + String(mesh.getNodeId()));
    node = this;
    alive = true;
}

void MeshNode::send_values(std::function<Measurements()> get_values_callback)
{
    // add it when we will have a sensor
    if (alive)
    {
        Measurements meas;
        meas = get_values_callback();
        String time1;
        if (AmPm == "AM")
        {
            time1 += (hours < 10) ? "0" + String(hours - 3) + ":" : "0" + String(hours - 3) + ":";

            time1 += (minutes < 10) ? "0" + String(minutes) + ":" : String(minutes) + ":";
            time1 += (seconds < 10) ? "0" + String(seconds) + ":" : String(seconds);
        }
        else
        {
            time1 += String(hours + 12 - 3) + ":";
            time1 += (minutes < 10) ? "0" + String(minutes) + ":" : String(minutes) + ":";
            time1 += (seconds < 10) ? "0" + String(seconds) + ":" : String(seconds);
        }
        // Serial.println(time1);
        String timeStamp = date + "T" + time1 + "Z";
        Serial.println("sendValues");
        DynamicJsonDocument measure1(256); // ameassure sample Json
        for (Measurement m : meas)
        {
            measure1["nodeId"] = mesh.getNodeId();
            measure1["sensorId"] = "sensor" + String(m.sensor_id);
            measure1["meassure_type"] = m.type;
            measure1["value"] = m.value;
            measure1["time"]["timestampValue"] = timeStamp;
            if (measure1["meassure_type"].as<String>() != "" && mesh.isConnected(bridgeId))
            {
                String castString = measure1.as<String>();
                Serial.println("read message:");
                Serial.println(measure1.as<String>());
                myqueue.push(measure1.as<String>());
            }
        }
    }
}

void MeshNode::add_measurement(std::function<Measurements()> callable, unsigned long interval, long iterations)
{
    Serial.println("adding measurement task");
    measure.set(TASK_SECOND * 30, TASK_FOREVER, [this, callable]()
                { send_values(callable); });
    userScheduler.addTask(measure);
    measure.enable();
}
