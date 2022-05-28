#include "mesh_garden.h"


void MeshGarden::save_configuration(String& config){
    Serial.println("saving configuration to file");
    File file = LittleFS.open("/config.json", "w");
    file.print(config);
    //delay(1);  check i f helping in case writing fails
    file.close();
}


bool MeshGarden::load_configuration(){
    FirebaseJson json;
    FirebaseJsonData data;
    File file = LittleFS.open("/config.json", "r");
    json.readFrom(file);
    json.get(data, "sensors");
    return false;
}

MeshGarden::MeshGarden(){
    LittleFS.begin();
}