#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

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
    Serial.println("reading configuration file");
    File file = LittleFS.open("/config.json", "r");

    // parse json data into variables
    String nickname;
    String firmware;
    String mesh_ssid;
    String mesh_password;
    int mesh_port = 0;
    ArduinoJson sensors;
    int sensor_id;
    int sample_interval;
    ArduinoJson units;
    String hardware_info;
    String type;
    String unit;

    Serial.println("Node Configurations:");
    Serial.print("    - ");
    Serial.print("Nickname: ");
    Serial.println(nickname);
    Serial.print("    - ");
    Serial.print("Firmware: ");
    Serial.println(firmware);
    Serial.print("    - ");
    Serial.println("Mesh Network: ");
    Serial.print("        - ");
    Serial.print("SSID: ");
    Serial.println(mesh_ssid);
    Serial.print("        - ");
    Serial.print("Password: ");
    Serial.println(mesh_password);
    Serial.print("        - ");
    Serial.print("Port: ");
    Serial.println(mesh_port);
    Serial.print("    - ");
    Serial.println("Sensors: ");

    for (size_t i = 0; i < sensors.size(); i++)
    {

        Serial.print("        - ");
        Serial.print(hardware_info);
        Serial.println(":");
        Serial.print("            - ");
        Serial.print("Id: ");
        Serial.println(sensor_id);
        Serial.print("            - ");
        Serial.print("Types[units]: ");
        for (size_t i = 0; i < sensor_type.size(); i++)
        {
            Serial.print(type);
            Serial.print("[");
            Serial.print(unit);
            Serial.print("], ");
        }
        Serial.println();

        Serial.print("            - ");
        Serial.print("Pinout: ");
        for (size_t i = 0; i < ; i++)
        {
        }
        Serial.println();

        Serial.print("            - ");
        Serial.print("Samples Interval: ");
        Serial.println(sample_interval);
    }
}

void loop()
{
    delay(5000);
}