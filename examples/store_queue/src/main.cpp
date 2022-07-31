#include <Arduino.h>
// #include <EEPROM.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <queue>

struct Time
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    bool Am = 1; // Am=1 refers to "Am", Am=0 refers to "Pm"
};

struct Measurement
{
    unsigned int sensor_id;
    String type;
    float value;
    Time time;
};

void print_meas(Measurement meas)
{
    Serial.println("sensor " + String(meas.sensor_id) + " " + meas.type + " value: " + String(meas.value) +
                   " time: " + String(meas.time.hours) + ":" + String(meas.time.minutes) + ":" + meas.time.seconds + " AM?" + meas.time.Am);
    // Serial.print("sensor " + String(meas.sensor_id));
    // Serial.print(" type: " + meas.type);
    // Serial.print(" value: " + String(meas.value));
    // Serial.print(" time: " + String(meas.time.hours));
    // Serial.print(":" + String(meas.time.minutes));
    // Serial.print(":" + String(meas.time.seconds));
    // Serial.print(" AM?" + String(meas.time.Am));
    // Serial.println();
}

#define MEAS_QUEUE_SIZE (sizeof(Time) + sizeof(Measurement) + 150) // estimation TODO make a better performance
void store_queue(int addr, std::queue<Measurement> &q)
{
    Serial.println("convert measurements queue to json..");
    DynamicJsonDocument doc(q.size() * MEAS_QUEUE_SIZE);
    JsonArray array = doc.to<JsonArray>();

    while (!q.empty())
    {
        JsonObject nested = array.createNestedObject();
        Measurement meas = q.front();
        print_meas(meas);
        nested["sensor_id"] = meas.sensor_id;
        nested["type"] = meas.type;
        nested["value"] = meas.value;
        nested["hours"] = meas.time.hours;
        nested["minutes"] = meas.time.minutes;
        nested["seconds"] = meas.time.seconds;
        nested["Am"] = meas.time.Am;
        q.pop();
    }
    Serial.println("capacity: " + String(doc.capacity()) + ", actual size: " + String(doc.memoryUsage()) + ", overflowed: " + String(doc.overflowed()));
    // Serial.println(doc.as<String>());

    Serial.println("saving measurements queue to file..");
    File file = LittleFS.open("/queue.json", "w");
    serializeJson(doc, file);
    Serial.println("save to file complete with:");
    Serial.println("File Size: " + String(file.size()));
    file.close();
}

void load_queue(int addr, std::queue<Measurement> &q)
{
    Serial.println("loading measurements queue file to json..");
    File file = LittleFS.open("/queue.json", "r");
    if (!file)
    {
        Serial.println("file open failed");
        return;
    }
    Serial.println("open file complete with:");
    Serial.println("File Size: " + String(file.size()));

    // getting the measurements 
    Serial.println("convert json to measurements queue ..");
    DynamicJsonDocument doc(file.size() * 2); // estimation TODO make a better performance
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    Serial.println("capacity: " + String(doc.capacity()) + ", actual size: " + String(doc.memoryUsage()) + ", overflowed: " + String(doc.overflowed()));

    JsonArray arr = doc.as<JsonArray>();
    for (JsonVariant j_meas : arr)
    {
        // Serial.println(j_meas.as<String>());
        // Serial.println(j_meas["sensor_id"].as<String>());
        Measurement meas;
        meas.sensor_id = j_meas["sensor_id"];
        meas.type = j_meas["type"].as<String>();
        meas.value = j_meas["value"];
        meas.time.hours = j_meas["hours"];
        meas.time.minutes = j_meas["minutes"];
        meas.time.seconds = j_meas["seconds"];
        meas.time.Am = j_meas["Am"];
        q.push(meas);
        print_meas(meas);
    }

    // free space
    // doc.~BasicJsonDocument(); // why fail and crush?
}

std::queue<Measurement> queue;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    if (!LittleFS.begin())
    {
        Serial.println("fail to start files system.");
        return; // exit(1);
    }
    else
    {
        Serial.println("started the file system");
    }

    Serial.println("pushing to queue..");
    size_t size = 5;
    for (size_t i = 0; i < size; ++i)
    {
        Time time;
        time.Am = 0;
        time.hours = 11;
        time.minutes = 29;
        time.seconds = 45;
        Measurement meas;
        meas.sensor_id = i;
        meas.time = time;
        meas.type = "type" + String(i);
        meas.value = float(i);
        queue.push(meas);
        print_meas(meas);
    }

    int addr = 30;
    Serial.println("storing queue..");
    store_queue(addr, queue);

    Serial.println("queue size: " + String(queue.size()));

    Serial.println("loading queue..");
    load_queue(addr, queue);

    Serial.println("printing queue..");
    while(!queue.empty()){
        print_meas(queue.front());
        queue.pop();
    }

}

void loop()
{
    delay(5000);
}