#include "mesh_garden.h"


uint8_t pin(String pin)
{
    return 1;//pins_map[pin].as<uint8_t>();
}


void MeshGarden::save_configuration(String &config)
{
    Serial.println("saving configuration to file");
    File file = LittleFS.open("/config.json", "w");
    file.print(config);
    // delay(1);  check i f helping in case writing fails
    file.close();
}

bool MeshGarden::load_configuration()
{
    Serial.println("open configuration file");
    File file = LittleFS.open("/config.json", "r");
    if (!file)
    {
        Serial.println("file open failed");
        return false;
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
        return false;
    }

    // free space and print memory usage
    size_t capacity_before = doc.memoryUsage();
    doc.garbageCollect();
    doc.shrinkToFit();
    size_t capacity_after = doc.capacity();
    Serial.println("deserializeJson complete with:");
    Serial.println("Capacity: " + String(capacity_after) + ", reduced by " + String(100 * capacity_after / capacity_before));
    Serial.println("Actual Memory Usage: " + String(doc.memoryUsage()));
    return true;
}

// TODO move to utils
void printIndent(int indent_level, String str)
{
    String indent = "";
    for (int i = 0; i < 4 * indent_level; i++)
        indent += ' ';
    indent += "- ";
    Serial.print(indent);
    Serial.println(str);
}

// parse the config and initialize class members
void MeshGarden::parse_config()
{
    // TODO consider print here only in debug mode. nickname,firmware needed only for printing. dont print password to console
    Serial.println("Node Configurations:");
    const char *nickname = config["nickname"]; // "tester"
    const char *firmware = config["firmware"]; // "esp8266_v0.1"
    // TODO set it on node/bridge after making set function for it and making inheriting
    mesh_prefix = config["mesh_prefix"].as<String>();     // "whateverYouLike"
    mesh_password = config["mesh_password"].as<String>(); // "somethingSneaky"
    mesh_port = config["mesh_port"].as<size_t>();         // 5555

    printIndent(1, "Nickname: " + String(nickname));
    printIndent(1, "Firmware: " + String(firmware));
    printIndent(1, "Mesh Network:");
    printIndent(2, "SSID: " + mesh_prefix);
    printIndent(2, "Password: " + mesh_password);
    printIndent(2, "Port: " + mesh_port);
    printIndent(1, "Sensors:");

    JsonArray sensors = config["sensors"];
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

        // Device* new_device = DeviceFactory::create(String(hardware_info));
        // device_list.insert(new_device);
    }
    config.~DynamicJsonDocument();
}

MeshGarden::MeshGarden() : config(0)
{
    // start file system
    if (!LittleFS.begin())
    {
        Serial.println("fail to start files system.");
        return; // exit(1);
    }
}

void MeshGarden::add_sensor(String hardware_id, InitSensor init_sensor_func, Measure masure_func)
{
}

void MeshGarden::update() {}