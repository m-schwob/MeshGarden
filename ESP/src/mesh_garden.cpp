#include "mesh_garden.h"

MeshGarden::GenericSensor::GenericSensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : Sensor(device_id, hardware_info, pinout, envelop) {}

void MeshGarden::GenericSensor::init_sensor()
{
    init_sensor_func();
}

Measurements MeshGarden::GenericSensor::measure()
{
    return measure_func();
}

void MeshGarden::GenericSensor::calibrate() {}

void MeshGarden::GenericSensor::set(Funcs funcs)
{
    this->init_sensor_func = funcs.init_sensor_func;
    this->measure_func = funcs.measure_func;
}

void MeshGarden::save_configuration(String &config)
{
    Serial.println("saving configuration to file");
    File file = LittleFS.open("/config.json", "w");
    file.print(config);
    // delay(1);  check i f helping in case writing fails
    file.close();
}

// load configuration from memory to json object
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
    config.~BasicJsonDocument();
    config = DynamicJsonDocument(file.size() * 3);
    // DynamicJsonDocument doc(file.size());
    DeserializationError error = deserializeJson(config, file);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    // free space and print memory usage
    size_t capacity_before = config.memoryUsage();
    config.garbageCollect();
    config.shrinkToFit();
    size_t capacity_after = config.capacity();
    Serial.println("deserializeJson complete with:");
    Serial.println("Capacity: " + String(capacity_after) + ", reduced by " + String(100 * capacity_after / capacity_before));
    Serial.println("Actual Memory Usage: " + String(config.memoryUsage()));

    Serial.println(config.as<String>());
    return true;
}

// parse the config and initialize class members
void MeshGarden::parse_config()
{
    Serial.println("start of parse config, the log config is:");
    // TODO calling it only in debug mode.
    log_config();
    Serial.println("log config done");

    JsonObject sensors = config["sensors"];
    for (JsonPair s : sensors)
    {
        JsonObject sensor = s.value().as<JsonObject>();
        const String hardware_info = sensor["hardware_info"].as<String>();
        const int sensor_id = sensor["sensor_id"].as<int>();
        // const int sample_interval = sensor["sample_interval"];

        DynamicJsonDocument doc(sensor);

        JsonObject pinout = sensor["pinout"];

        Serial.println("create sensor: " + hardware_info);
        Device *new_device = DeviceFactory::create(sensor_id, hardware_info, pinout, doc);
        if (new_device)
        {
            // check if the new device is a simple user define sensor
            std::map<String, Funcs>::const_iterator iter = funcs_map.find(hardware_info);
            if (iter != funcs_map.end())
            {
                if (new_device->DEVICE_TYPE.equals(DEVICE_TYPE_SENSOR))
                {
                    ((GenericSensor *)new_device)->set((*iter).second);
                }
            }
            device_list.push_front(new_device);
        }
        else // if null, means create fails
        {
            Serial.println("fail to initialize sensor " + hardware_info);
        }
    }
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

// print config file nicely to log
void MeshGarden::log_config()
{
    // TODO replace string keys with constants
    Serial.println("Node Configurations:");

    const char *nickname = config["nickname"]; // "tester"
    // const char *firmware = config["firmware"];           // "esp8266_v0.1"
    // const char *mesh_prefix = config["mesh_prefix"];     // "whateverYouLike"
    // const char *mesh_password = config["mesh_password"]; // "somethingSneaky"
    // const int mesh_port = config["mesh_port"];           // 5555

    printIndent(1, "Nickname: " + String(nickname));
    // printIndent(1, "Firmware: " + String(firmware));
    // printIndent(1, "Mesh Network:");
    // printIndent(2, "SSID: " + String(mesh_prefix));
    // printIndent(2, "Password: " + String(mesh_password));
    // printIndent(2, "Port: " + String(mesh_port));
    printIndent(1, "Sensors:");

    JsonObject sensors = config["sensors"];
    for (JsonPair s : sensors)
    {
        // Serial.println(sensors.as<String>());
        JsonObject sensor = s.value().as<JsonObject>();
        const char *hardware_info = sensor["hardware_info"];   // " DHT22"
        const int sensor_id = sensor["sensor_id"];             // 1
        const int sample_interval = sensor["sample_interval"]; // 30

        printIndent(2, String(hardware_info) + ':');
        printIndent(3, "Id: " + String(sensor_id));
        printIndent(3, "Sample Interval: " + String(sample_interval));
        printIndent(3, "Measurement Types [units]:");

        JsonArray sensor_type = sensor["Measurement_type"];
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

void MeshGarden::init_power_monitor()
{
    StaticJsonDocument<100> pinout;
    char json[] = "{\"BAT\":\"ADS3\"}";
    deserializeJson(pinout, json);
    power_monitor = new PowerMonitorSensor(0, "", pinout.as<JsonObject>(), DynamicJsonDocument(0));
}

void MeshGarden::init_mesh_connection()
{
    // choose mesh node type to construct
#if defined(ESP32)
    network = new MeshBridge();
#else
    network = new MeshNode();
#endif

    // init mesh network
    network->set_global_config(config["network_config"]);
    network->init_clock();
   
#if defined(ESP32)
    network->firebaseNetworkSet(config);
#endif 

#if defined(ESP8266)
    for (Device *device : device_list)
    {
        if (device->DEVICE_TYPE.equals(DEVICE_TYPE_SENSOR))
        {
            network->add_measurement(((Sensor *)device)->get_measure_callback(), 5, 5);
        }
    }
#endif
    network->init_mesh();

    Serial.println("init mesh connection done");
#if defined(ESP8266)
    network->call_measurements();
#endif

}

MeshGarden::MeshGarden() : config(0)
{
    // Serial.begin(115200);
}

void MeshGarden::add_sensor(String hardware_info, InitSensor init_sensor_func, Measure measure_func)
{
    Funcs funcs;
    funcs.init_sensor_func = init_sensor_func;
    funcs.measure_func = measure_func;
    funcs_map[hardware_info] = funcs;
}

void MeshGarden::begin()
{
    pinMode(LED_BUILTIN, OUTPUT);

#if defined(ESP32)
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)

#else
    digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
#endif
    // create a pins map
    map_pins();

    // start file system
    if (!LittleFS.begin())
    {
        Serial.println("fail to start files system.");
        return; // exit(1);
    }
    else
    {
        Serial.println("started the file system");
    }
#if defined(ESP8266) // TODO solve differences with power monitor
    ads.begin();
#endif
    load_configuration();
    parse_config();
    Serial.println("serialization done, now init mesh");
#if defined(ESP8266) // TODO solve differences with power monitor
    init_power_monitor();
#endif

    init_mesh_connection();

#if defined(ESP8266) // TODO solve differences with power monitor
    Measurements power_status = power_monitor->measure_wrapper();
    for (Measurement meas : power_status)
    {
        if (meas.type == BATTERY_LEVEL_KEY)
            network->get_battery_level(meas);
    }
#endif
    config.~DynamicJsonDocument();
}

std::list<String> MeshGarden::get_device_list()
{
    std::list<String> list;
    for (Device *device : device_list)
    {
        list.push_back(device->HARDWARE_INFO);
    }
    return list;
}

void MeshGarden::update()
{
    if (network->configure_ready)
    {
        Serial.println("configure ready");
        save_configuration(network->config_string);
        network->configure_ready = false;
        Serial.println("entring delay");
        delay(2000);
        Serial.println("reset");
        ESP.restart();
    }
    network->update();
}

