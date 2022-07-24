#include "mesh_bridge.h"

//input: NULL
//output: the function will print the time of day set in rtc
void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
static MeshBridge *node = NULL;
MeshBridge::MeshBridge()
{
    node = this;
}


// Needed for painless library
// receivedCallback - when we get a post on the mesh, it will update the dict by the message sent to it
// the dict will have nodeId as its key, and the Entire message as its value
void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, msg);
    if (doc.containsKey("battery")){
        node->battery_map[String(from)] = doc["battery"].as<float>() ;
    }
    else{
    node->server_data.push(doc.as<String>());
    serializeJson(doc, Serial);
    Serial.println("recieved measure from node "+ String(from));
    }
}
// event driven function for the mesh
void newConnectionCallback(uint32_t nodeId)
{
    uint32_t time = node->mesh.getNodeTime();
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time try again");
        return;
    }
    char s[100];

    int rc = strftime(s, sizeof(s), "20%y--%d at %r", &timeinfo);
    String firebaseReadyDate = String(s);
    int monthId = timeinfo.tm_mon + 1;
    if (monthId < 10)
        firebaseReadyDate = firebaseReadyDate.substring(0, 5) + "0" + String(monthId) + firebaseReadyDate.substring(5);
    else
        firebaseReadyDate = firebaseReadyDate.substring(0, 5) + String(monthId) + firebaseReadyDate.substring(5);

    DynamicJsonDocument timer(firebaseReadyDate.length() + 32);
    timer["clock"] = firebaseReadyDate;
    Serial.println("created clock json");
    serializeJson(timer, Serial);
    node->mesh.sendBroadcast(timer.as<String>());

    DynamicJsonDocument death_time(NEXT_DEATH_JSON);
    death_time["d"]["m"] = node->die_minutes;
    death_time["d"]["h"] = node->die_hours;
    death_time["d"]["s"] = node->die_seconds;
    death_time["d"]["st"] = node->NODE_DEEP_SLEEP_TIME; // send the sleep time
    node->mesh.sendBroadcast(death_time.as<String>());
    Serial.println("next death time:" + death_time.as<String>());

    if (node->change_log.find(String(nodeId)) != node->change_log.end())
    {
        Serial.println("update node " + String(nodeId) + "\n" + String(node->change_log[String(nodeId)]));
        if (node->change_log[String(nodeId)] != "")
        {
            DynamicJsonDocument doc(node->change_log[String(nodeId)].length() + 32);
            doc["change"] = node->change_log[String(nodeId)];
            serializeJson(doc, Serial);
            node->mesh.sendSingle(nodeId, doc.as<String>());
            node->change_log.erase(String(nodeId));
        }
    }
}
// event driven functions for the mesh
void changedConnectionCallback()
{
    Serial.printf("Changed connections\n");
}
// event driven functions for the mesh
void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", node->mesh.getNodeTime(), offset);
}


//input: Json contains the configuration of the Node
//output: NONE
//the function will save the values of parameters in the flash memory
void MeshBridge::set_global_config(JsonObject global_config)
{
    // mesh settings
    MESH_PREFIX = global_config["mesh"]["mesh_prefix"].as<String>();
    MESH_PASSWORD = global_config["mesh"]["mesh_password"].as<String>();
    MESH_PORT = global_config["mesh"]["mesh_port"].as<size_t>();

    // wifi settings
    ssid = new char[100];
    ssid = new char[global_config["wifi"]["ssid"].as<String>().length()];
    strcpy(ssid, global_config["wifi"]["ssid"]);
    // password = new char[100];
    password = new char[global_config["wifi"]["password"].as<String>().length()];
    strcpy(password, global_config["wifi"]["password"]);

    // time settings
    // ntp_server = new char[100];
    ntp_server = new char[global_config["time"]["ntp_server"].as<String>().length()];
    strcpy(ntp_server, global_config["time"]["ntp_server"]);
    gmt_offset_sec = global_config["time"]["gmt_offset_sec"].as<long>();
    daylight_offset_sec = global_config["time"]["daylight_offset_sec"].as<int>();

    // firebase settings
    API_KEY = global_config["firebase"]["api_key"].as<String>();
    FIREBASE_PROJECT_ID = global_config["firebase"]["firebase_project_id"].as<String>();
    USER_EMAIL = global_config["firebase"]["user_email"].as<String>();
    USER_PASSWORD = global_config["firebase"]["user_password"].as<String>();
    
    //deep sleep settings
    NODE_WAKE_TIME = global_config["mesh_timing"]["mesh_connection_time"].as<int>();
    NODE_DEEP_SLEEP_TIME=global_config["mesh_timing"]["deep_sleep_time"].as<int>();
    Serial.println("config done");
}

void MeshBridge::init_mesh()
{
    Serial.println("initializeing the mesh network");
    myTime = millis();
    // mesh.setDebugMsgTypes( ERROR | CONNECTION | COMMUNICATION ); // all types on
    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onDroppedConnection([](uint32_t nodeId)
                             {
   // Do something with the event
   Serial.print("changed connectioמ with node" + String(nodeId)+ "at time: "+ node->mesh.getNodeTime()); });
    mesh.onNodeDelayReceived([](uint32_t nodeId, int delay)
                             {
       // Do something with the event
    Serial.println(String(delay)); });
    // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
    mesh.setRoot(true);
    // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
    mesh.setContainsRoot(true);
}

void MeshBridge::update()
{
    // it will run the user scheduler as well
    if (got_time)
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            return;
        }
        if (timeinfo.tm_hour == die_hours && timeinfo.tm_sec == die_seconds && timeinfo.tm_min == die_minutes)
        {
            get_mesh_nodes(); // get the working nodes list before quit
            mesh.setContainsRoot(false);
            lasttime = millis();
            digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
            exit_mesh_connect_server();
        }
    }
    mesh.update();
}

/*
input: NONE
output: NONE
an initialization function: 
the bridge will connect to WIFI and get the time of day by NTP protocol, then set the rtc with that data
*/
void MeshBridge::init_clock()
{
    Serial.print("Connecting to server to calibrate clock");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");

    // Init and get the time
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    struct tm timeinfo;
    printLocalTime();
    calculate_death(NODE_WAKE_TIME);
    Serial.println("time of next death is:\n");
    Serial.printf("%d:%d:%d", die_hours, die_minutes, die_seconds);
    got_time = true;
    // disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

// fireBase Init function, insert the Project firestore ID and do authentication by userName and password
void MeshBridge::firebaseInit()
{
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    Firebase.begin(&config, &auth);
}

/*
input: a JSON object representing a sample
output: NONE
fireBase update Function For sensor measurements and post it on the firebase
*/
void MeshBridge::firestoreDataUpdate(String jsonVal) // TEMP parameters edited have measurement type and value
{
    if (Firebase.ready())
    {
        DynamicJsonDocument doc(1028);
        DeserializationError error = deserializeJson(doc, jsonVal);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
            }

        // The dyamic array of write object fb_esp_firestore_document_write_t.
        std::vector<struct fb_esp_firestore_document_write_t> writes;

        // A write object that will be written to the document.
        struct fb_esp_firestore_document_write_t transform_write;
        // Set the write object write operation type.
        // fb_esp_firestore_document_write_type_update,
        // fb_esp_firestore_document_write_type_delete,
        // fb_esp_firestore_document_write_type_transform
        transform_write.type = fb_esp_firestore_document_write_type_transform;

        // Set the document path of document to write (transform)
        transform_write.document_transform.transform_document_path = "Measurements/" + doc["nodeId"].as<String>();

        // Set a transformation of a field of the document.
        struct fb_esp_firestore_document_write_field_transforms_t field_transforms;
        // Set field path to write. 
        
        String f_path = doc["sensorId"].as<String>()+ "." + doc["meassure_type"].as<String>() + ".samples";
        // field_transforms.fieldPath = doc["sensorId"].as<String>()+ ".Soil_Moisture.samples";
        field_transforms.fieldPath = f_path;

        
        // field_transforms.fieldPath = "appended_data";

        // Set the transformation type.
        // fb_esp_firestore_transform_type_set_to_server_value,
        // fb_esp_firestore_transform_type_increment,
        // fb_esp_firestore_transform_type_maaximum,
        // fb_esp_firestore_transform_type_minimum,
        // fb_esp_firestore_transform_type_append_missing_elements,
        // fb_esp_firestore_transform_type_remove_all_from_array
        field_transforms.transform_type = fb_esp_firestore_transform_type_append_missing_elements;

        // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
        FirebaseJson content;
        content.set("values/[0]/mapValue/fields/time/timestampValue/" ,doc["time"]["timestampValue"].as<String>());
        content.set("values/[0]/mapValue/fields/value/doubleValue/",doc["value"].as<double>());

        // Set the transformation content.
        field_transforms.transform_content = content.raw();

        // Add a field transformation object to a write object.
        transform_write.document_transform.field_transforms.push_back(field_transforms);

        // Add a write object to a write array.
        writes.push_back(transform_write);

        if (Firebase.Firestore.commitDocument(&fbdo, FIREBASE_PROJECT_ID, "", writes /* dynamic array of fb_esp_firestore_document_write_t */, "" /* transaction */))
            Serial.printf("commit succeeded\n");
        else
            Serial.println(fbdo.errorReason());
    }        
}

void MeshBridge::firestoreUpdateLastMesh(String jsonVal){
   if (Firebase.ready())
    {
        DynamicJsonDocument doc(1028);
        DeserializationError error = deserializeJson(doc, jsonVal);
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }
        FirebaseJson content;

        // aa is the collection id, bb is the document id.
        String documentPath = "Measurements/" + doc["nodeId"].as<String>();

        String f_path = "fields/"+doc["sensorId"].as<String>()+ "/mapValue/fields/" + doc["meassure_type"].as<String>() + "/mapValue/fields/newSample/mapValue/fields/";
        content.set(f_path + "time/timestampValue/" ,doc["time"]["timestampValue"].as<String>());
        content.set(f_path + "value/doubleValue/",doc["value"].as<double>());
        String masker = doc["sensorId"].as<String>()+"."+doc["meassure_type"].as<String>()+".newSample";
        
        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), masker /* updateMask */))
           { 
            Serial.println("set last_sample for node:" + doc["nodeId"].as<String>());
           }
        else
            Serial.println(fbdo.errorReason());
    }
}        

/*
input: NONE
output:NONE
the function uses the painlessmesh library's function getNodeList to get evey node that connected to the bridge,
it saves the nodes that are active in the mesh network before the bridge disconnect
*/
void MeshBridge::get_mesh_nodes()
{
    Serial.printf("mesh nodes: ");
    list<uint32_t>::iterator i;
    list<uint32_t> map_node = mesh.getNodeList();
    for (i = map_node.begin(); i != map_node.end(); ++i)
    {
        Serial.printf("mesh network contains: %s," ,String(*i));
        mesh_values.push_back(String(*i));
    }
    Serial.printf("%s",String(mesh.getNodeId()));
    mesh_values.push_back(String(mesh.getNodeId()));
    ;
}

/*
input: NONE
output: NONE
the function will delete the document that contains active nodes before overwrite it and set the current active nodes
*/
void MeshBridge::firestoreMeshCollectionClear(){
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
    }
        String documentPath = "MeshNetwork/active";
        FirebaseJson content;
    if (Firebase.Firestore.deleteDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
    {
        Serial.printf("clear collection success\n");
        return;
    }else{
      Serial.println(fbdo.errorReason());
    }
    }

/*
input: NONE
output: NONE
the function will set the current active nodes in a document
*/
void MeshBridge::firestoreMeshCollectionUpdate(){

    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
        //rewrite the active nodes doc in the DB to contain the active nodes at given time:
        String documentPath = "MeshNetwork/active";
        FirebaseJson content;
        bool response;

        for(list<String>:: iterator map_iter = mesh_values.begin(); map_iter!= mesh_values.end(); ++map_iter){
            // Serial.println("send to server " + (*map_iter));
            content.set("fields/" + (*map_iter)+ "/nullValue");
        }
        if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
            Serial.printf("mesh collection update success\n");
            return;
        }else{
            Serial.println(fbdo.errorReason());
        }
    }
}

/*
input: NONE
output: NONE
the function will go over all the active nodes in the network and will check if there are any changes in the nodes
it will read them, store the change in a map and will delete the change from the database. 
when the node will reconnect to the bridge it will recieve the changes and restart  
*/
void MeshBridge::firestoreReadChanges()
{
    Serial.println("read changes");
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        for (list<String>::iterator iter = mesh_values.begin(); iter != mesh_values.end(); ++iter)
        {
            String documentPath = "/Changes/" + (*iter);
            FirebaseJson content;
            FirebaseJson json;
            bool response;
            FirebaseJsonData data;
            Serial.printf("check change for %s\n", documentPath.c_str());
            // if we get a document here hence the node_id has gone through changes.
            if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
            {
                // now we will scan the Nodes collection in order to retrieve all changes:
                Serial.printf("read change\n");
                DynamicJsonDocument doc(fbdo.payload().length());
                DeserializationError error = deserializeJson(doc, fbdo.payload());
                if (error)
                {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.f_str());
                }
                String changeLog1 = doc["fields"]["config"]["stringValue"].as<String>();
                String changeid;
                String network_data;
                // if( get_node_changes((*iter), changeid) && firestoreReadNetwork(network_data))
                change_log[(*iter)] = changeLog1;
                
                // A CODE TO DELETE A CHANGE THAT HAS BEEN READ:: for now its in comment cause the insertion is manual
                if (Firebase.Firestore.deleteDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
                {
                    Serial.printf("deleted\n");
                    return;
                }
                else
                {
                    Serial.println(fbdo.errorReason());
                }
            }
        }
    }
}

/*
input: NONE
output: NONE
the function will set the network details in the database
*/
void MeshBridge::firestoreNetworkDataCollectionUpdate()
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        String documentPathNetwork = "Network/mesh";
        FirebaseJson content_m;
        bool response2;
        String prefix = MESH_PREFIX;
        String password = MESH_PASSWORD;
        int port = MESH_PORT;
        content_m.set("fields/mesh_prefix/stringValue/", prefix.c_str());
        content_m.set("fields/mesh_password/stringValue/", password.c_str());
        content_m.set("fields/mesh_port/integerValue/", port);

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPathNetwork.c_str(), content_m.raw()))
        {
            Serial.printf("network data collection update succeeded");
            return;
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}

/*
bool MeshBridge::firestoreReadNetwork(String &changes)
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        String document_path = "Network/mesh/";
        FirebaseJson json;
        FirebaseData fbdo;
        FirebaseJsonData data;
        int response_size = 2048;
        bool succeed;
        do
        {
            response_size *= 2;
            Serial.println(String(response_size));
            fbdo.setResponseSize(response_size);
            if (!Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str()))
            {
                break;
            }
            // succeed = json.setJsonData(fbdo.payload()) && json.get(data, "fields") && data.getJSON(json);
            Serial.println(succeed);
            if (!json.setJsonData(fbdo.payload()))
            {
                Serial.println("set json fail");
                continue;
            }
            succeed = json.setJsonData(fbdo.payload());
            Serial.println(succeed);
            succeed = json.get(data, "fields");
            Serial.println(succeed);
            succeed = data.getJSON(json);
            Serial.println(succeed);
            // Serial.println(json.raw());
            if (succeed)
            {
                changes = json.raw();
                return true;
            }
        } while (!succeed && response_size < 16384); // TODO consider this number again
    }
    Serial.println("data failed");
    Serial.println(fbdo.errorReason());
    return false;
}
*/

void MeshBridge::set_bridge_in_firebase(String nodeId)
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        // part1: initializing empty Node in the node list dedicated for the bridge
        String documentPath = "Nodes/" + nodeId;
        FirebaseJson content;
        bool response;
        content.set("fields/nickname/stringValue/", nodeId.c_str());
        content.set("fields/bridge/booleanValue/", true);
        content.set("fields/active/booleanValue/", true);
        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
        {
            Serial.printf("initializing bridge succeed\n");
            initialized = true;
            return;
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}
void MeshBridge::set_in_firebase(String nodeId)
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        // part1: initializing empty Node in the node list dedicated for the bridge
        String documentPath = "Nodes/" + nodeId;
        FirebaseJson content;
        bool response;
        content.set("fields/nickname/stringValue/", nodeId.c_str());
        content.set("fields/bridge/booleanValue/", false);
        content.set("fields/active/booleanValue/", true);
        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
        {
            Serial.printf("initialize node:%s\n",nodeId.c_str());
            // initialized = true;
            set_default_nickname(nodeId);
            return;
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}


void MeshBridge::exit_mesh_connect_server()
{
    String nodeId = String(mesh.getNodeId());
    mesh.stop();
    calculate_death(NODE_WAKE_TIME+NODE_DEEP_SLEEP_TIME);
    // // Connect to Wi-Fi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    firebaseInit();

    //*************initialize Nodes collections:****************//
    // set initialized values for the mesh network config and the bridge
    if (!initialized)
    {
        set_bridge_in_firebase(nodeId);
        firestoreNetworkDataCollectionUpdate();
    }
    // go through the nodes in the system and will set uninitialized nodes in the NodesDocument:
    Serial.println("initialize:");
    for (list<String>::iterator map_iter = mesh_values.begin(); map_iter != mesh_values.end(); ++map_iter)
    {
        Serial.println("we want to initialize the node: " + String(*map_iter));
        set_in_firebase(*map_iter);
    }
    //*************initialize Nodes collections completed****************//


    // //  //pst on the firebase server what was saved on the map
    // for(std::map<String,vector<String>>::iterator iter = dict.begin(); iter != dict.end(); ++iter){
    // for(vector<String>::iterator measures_iter = iter->second.begin(); measures_iter!=iter->second.end() ; measures_iter++ ){
    // firestoreDataUpdate(iter->first, *measures_iter);
    //}
    //}
    // refresh the MeshNetwork and initializes the New Nodes in the network

    Serial.println("update connections");
    firestoreMeshCollectionClear();
    firestoreMeshCollectionUpdate();


    //*************updating Measurements and battery collections****************//
    Serial.printf("sending %d cached messages:\n", server_data.size());
    // Serial.println(server_data.front());

    while (!server_data.empty())
    {
        String front = server_data.front();
        firestoreDataUpdate(front);
        if(server_data.size()==1){
            firestoreUpdateLastMesh(front);
        }
        server_data.pop();
    }

    Serial.printf("sending %d battey messages..\n", battery_map.size());
    for(std::map<String,float>::iterator iter = battery_map.begin(); iter != battery_map.end(); ++iter)
    {
        firestoreMapBatteryUpdate(iter->first,iter->second);
    }
    battery_map.clear();
    //*************updating Measurements and collections****************//

    // Serial.println("send to server list:");
    firestoreReadChanges();
    mesh_values.clear();
    Serial.println("send to server end");
    Serial.println("WiFi disconnected. initialize mesh");
    // initializing the mesh network again
    init_mesh();
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
}

void MeshBridge::set_default_nickname(String nodeId)
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        String documentPath = "NodesNickname/" + nodeId;
        FirebaseJson content;
        bool response;
        content.set("fields/nodeId/stringValue/", nodeId.c_str());

        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(),"nodeId"))
        {
            Serial.printf("defauld nickname set");
            return;
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}

/*
input: integer representing time to death in seconds
output: NONE
by getting ttd from the user the function will calculate the next time of death for all the nodes in the system
*/
void MeshBridge::calculate_death(int ttd)
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    die_seconds = (timeinfo.tm_sec+ttd)%60;
    die_minutes = ((timeinfo.tm_min+ ((timeinfo.tm_sec+ttd)/60))%60);
    die_hours = ((timeinfo.tm_hour + ((timeinfo.tm_min+((timeinfo.tm_sec+ttd)/60))/60)));
    return;
}

MeshBridge::~MeshBridge()
{
    delete ssid;
    delete password;
    delete ntp_server;
}

void MeshBridge::get_battery_level(Measurement battery_level){}

void MeshBridge::firestoreMapBatteryUpdate(String nodeId , float value){
       if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        String documentPath = "Nodes/" + nodeId;
        FirebaseJson content;
        bool response;
        content.set("/fields/battery/floatValue/", value);
        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "battery"))
        {
            Serial.printf("battery update succeded");
            return;
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}
