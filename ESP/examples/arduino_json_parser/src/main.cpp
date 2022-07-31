#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

void printIndent(int indent_level, String str)
{
    String indent = "";
    for (int i = 0; i < 4 * indent_level; i++)
        indent += ' ';
    indent += "- ";
    Serial.print(indent);
    Serial.println(str);
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    // start file system
    if (!LittleFS.begin())
    {
        Serial.println("fail to start files system.");
        exit(1);
    }

    // read pre uploaded file
    Serial.println("open configuration file");
    File file = LittleFS.open("/config.json", "r");
    if (!file)
    {
        Serial.println("file open failed");
        return;
    }
    Serial.println("open file complete with:");
    Serial.println("File Size: " + String(file.size()));

    // init JsonDocument Object //

    // TBD and following check. The capacity calculated by ArduinoJson Assistant on a sample was 869.
    // Following the FAQ recommendation the choosen capacity is 2048 (instead  the solution below is done, needs following).
    // It should be reconsider based on real cases and additional code for preventing it from failing
    // in run time should be added. consider (done this solution) using the file size as base size and them use fit function.
    // https://arduinojson.org/v6/assistant/
    // https://arduinojson.org/v6/how-to/determine-the-capacity-of-the-jsondocument/
    DynamicJsonDocument doc(file.size());
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    // free space and print memory usage
    Serial.println("deserializeJson complete with:");
    Serial.println("Total Capacity: " + String(doc.capacity()));
    Serial.println("Actual Memory Usage: " + String(doc.memoryUsage()));

    doc.garbageCollect();
    Serial.println("collecting garbage complete with:");
    Serial.println("Total Capacity: " + String(doc.capacity()));
    Serial.println("Actual Memory Usage: " + String(doc.memoryUsage()));

    doc.shrinkToFit();
    Serial.println("collecting garbage complete with:");
    Serial.println("Total Capacity: " + String(doc.capacity()));
    Serial.println("Actual Memory Usage: " + String(doc.memoryUsage()));

    Serial.println("Node Configurations:");

    const char *nickname = doc["nickname"];           // "tester"
    const char *firmware = doc["firmware"];           // "esp8266_v0.1"
    const char *mesh_prefix = doc["mesh_prefix"];     // "whateverYouLike"
    const char *mesh_password = doc["mesh_password"]; // "somethingSneaky"
    const int mesh_port = doc["mesh_port"];           // 5555

    printIndent(1, "Nickname: " + String(nickname));
    printIndent(1, "Firmware: " + String(firmware));
    printIndent(1, "Mesh Network:");
    printIndent(2, "SSID: " + String(mesh_prefix));
    printIndent(2, "Password: " + String(mesh_password));
    printIndent(2, "Port: " + String(mesh_port));
    printIndent(1, "Sensors:");

    JsonArray sensors = doc["sensors"];
    for (JsonObject sensor : sensors)
    {
        const char *hardware_info = sensor["hardware_info"];   // " DHT22"
        const int sensor_id = sensor["sensor_id"];             // 1
        const int sample_interval = sensor["sample_interval"]; // 30

        printIndent(2, String(hardware_info) + ':');
        printIndent(3, "Id: " + String(sensor_id));
        printIndent(3, "Sample Interval: " + String(sample_interval));
        printIndent(3, "Measurement Types [units]:");

        JsonArray sensor_type = sensor["sensor_type"];
        JsonArray units = sensor["units"];
        for (JsonArray::iterator type = sensor_type.begin(), unit = units.begin();
             type != sensor_type.end() && unit != units.end(); ++type, ++unit)
        {
            (*type).as<String>(); // "Air Humidity"
            (*unit).as<String>(); // "%"
            printIndent(4, (*type).as<String>() + " [" + (*unit).as<String>() + ']');
        }

        printIndent(3, "Pinout:");
        JsonObject pinout = sensor["pinout"];
        for (JsonPair pin : pinout)
        {
            pin.key();   //"DAT"
            pin.value(); // "D1"
            printIndent(4, String(pin.key().c_str()) + " --> " + pin.value().as<String>());
        }
    }
}

void loop()
{
    delay(5000);
}