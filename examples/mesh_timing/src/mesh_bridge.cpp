#include "mesh_bridge.h"

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

static MeshBridge *node = NULL;

MeshBridge::MeshBridge(){
    init_clock();
    init_mesh();
    node = this;
}

// Needed for painless library
// receivedCallback - when we get a post on the mesh, it will update the dict by the message sent to it
// the dict will have nodeId as its key, and the Entire message as its value
void receivedCallback(uint32_t from, String &msg)
{
    uint32_t timer = node->mesh.getNodeTime();
    Serial.printf("message received from %u msg=%s. at time: %u\n", from, msg.c_str(),timer);
    // node->server_data.push_back(String(from) + "," + msg);
}

// event driven function for the mesh
void newConnectionCallback(uint32_t nodeId)
{
    uint32_t time = node->mesh.getNodeTime();
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    Serial.printf("new node connected at time: %u\n", time);
    Serial.println("time:");    
    printLocalTime();
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
        }
    char s[100];
    
    int rc = strftime(s,sizeof(s),"%b %d,20%y at %r", &timeinfo);
    Serial.printf("%d characters written.\n%s\n",rc,s);   
    node->mesh.sendBroadcast("clock " + String(s));

    if (node->change_log[String(nodeId)]){
        Serial.println("sending update to node " + String(nodeId));
        node->mesh.sendSingle(nodeId,"Change: " +node-> change_log[String(nodeId)]);
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

void MeshBridge::init_mesh()
{   
     myTime = millis();
    // Serial.printf("Mesh node start at time: %d \n", myTime);
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
   Serial.print("changed connectio with node" + String(nodeId)+ "at time: "+ node->mesh.getNodeTime());
    });
   mesh.onNodeDelayReceived([](uint32_t nodeId, int delay) {
       // Do something with the event
       Serial.println(String(delay));
    });
    // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
    mesh.setRoot(true);
    // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
    mesh.setContainsRoot(true);
}

void MeshBridge::update()
{
      // it will run the user scheduler as well
    mesh.update();
    // Init and get the time
    if (millis() - lasttime > 30000)
    {
        get_mesh_nodes(); // get the working nodes list before quit
        String nodeId = String(mesh.getNodeId());
        mesh.sendBroadcast("die");
        mesh.stop();
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

        //set initialized values for the mesh network config and the bridge
        if(!initialized){
            set_in_firebase(nodeId);
            firestoreNetworkDataCollectionUpdate();
        }

        // //  //pst on the firebase server what was saved on the map
        // for(std::map<String,vector<String>>::iterator iter = dict.begin(); iter != dict.end(); ++iter){
        // for(vector<String>::iterator measures_iter = iter->second.begin(); measures_iter!=iter->second.end() ; measures_iter++ ){
        // firestoreDataUpdate(iter->first, *measures_iter);
        //}
        //   }

        //refresh the MeshNetwork and initializes the New Nodes in the network
        Serial.println("update connections");
        firestoreMeshCollectionClear();
        firestoreMeshCollectionUpdate();
        
        //go through the nodes in the system and will set uninitialized nodes in the NodesDocument:
        Serial.println("initialize:");
        for(list<String>:: iterator map_iter = mesh_values.begin(); map_iter!= mesh_values.end(); ++map_iter){
            Serial.println("we want to initialize the node: " + String(*map_iter));
            set_in_firebase(*map_iter);
        }

        //sending to the firstore the meassures:
        Serial.printf("sending %d cached messages..\n", server_data.size());
        for (vector<String>::iterator measures_iter = server_data.begin(); measures_iter != server_data.end(); measures_iter++)
        {
            Serial.printf("sending message <%s>\n", measures_iter->c_str());
            std::vector<String> vec = split((*measures_iter), ","); // split the String to vector of Strins by word
            firestoreDataUpdate(String(vec[0].c_str()),String(vec[1].c_str()), String(vec[2].c_str()), String(vec[3].c_str()));
        }
        
        server_data.clear();
        //Serial.println("send to server list:");
        firestoreReadChanges();	

        mesh_values.clear();
        Serial.println("send to server end");
        Serial.println("WiFi disconnected. initialize mesh");
        
        // initializing the mesh network again
        init_mesh();
    Serial.print("after update after entering fb heap: ");
        lasttime = millis();
    }
}

void MeshBridge::init_clock(){
  Serial.print("Connecting to server to calibrate clock");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

}

// a function that parses a string and creates a vector of words
vector<String> MeshBridge::split(String _s, String _delimiter)
{   
    string s = _s.c_str();
    string delimiter = _delimiter.c_str();
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<String> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token.c_str());
    }
    res.push_back(s.substr(pos_start).c_str());
    return res;
}

// fireBase Init function, insert the Project firestore ID and do authentication by userName and password
void MeshBridge::firebaseInit()
{
    Serial.println("firebaseKeys:");
    Serial.println(API_KEY);
    Serial.println(USER_EMAIL);
    Serial.println(USER_PASSWORD);
    Serial.println("finished");

    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    Firebase.begin(&config, &auth);
}

// fireBase update Function For moisture and humidity sensor, will receive 3 Values
//  (Have to fix temp- for now its the nodeId), and post them on the firebase
// under the plant_id

// Todo- check option for sensor to sand Json message on mesh, and firestore will decode it
// declare Strings in each sensor class
void MeshBridge::firestoreDataUpdate(String plant_id, String sensor_id ,String meas_type, String value) //TEMP parameters edited have measurement type and value 
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        Serial.printf("updating firebase with data: %s:%s\n", meas_type.c_str(), value.c_str());
        String document_path = "Measurements/" + plant_id +"/"+ sensor_id +"/meassurments";
        FirebaseJson content;
        bool response;

        content.set("fields/" + plant_id + "/mapValue/fields/" + meas_type.c_str()+ "/doubleValue/", value.c_str());
        // check if node has a document nad create new if not exists
        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str(), plant_id.c_str())){
            Serial.printf("node document found\n%s\n\n", fbdo.payload().c_str());
            response = Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str(), content.raw(), "last_message");
        }
        if (response)
        {
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}


//this function will first change the network document in the DB, 
//it will switch the active node according to the nodes that are alive on the system
//then it will create uninitialize document for the nodes that did not entered the DB yet.
//the user will initialize it with the application.
void MeshBridge::firestoreMeshCollectionUpdate(){

    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
        //rewrite the active nodes doc in the DB to contain the active nodes at given time:
        String documentPath = "MeshNetwork/active";
        FirebaseJson content;
        bool response;

        for(list<String>:: iterator map_iter = mesh_values.begin(); map_iter!= mesh_values.end(); ++map_iter){
            Serial.println("send to server " + (*map_iter));
            content.set("fields/" + (*map_iter)+ "/nullValue");
        }
        if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        }else{
            Serial.println(fbdo.errorReason());
        }
    }
}


void MeshBridge::get_mesh_nodes()
{
    list<uint32_t>::iterator i;
    list<uint32_t> map_node = mesh.getNodeList();
    for(i=map_node.begin() ; i!= map_node.end(); ++i){
        Serial.println("mesh network contains:" + String(*i));
        mesh_values.push_back(String(*i));
    }
    Serial.println("mesh network bridge:" + String(mesh.getNodeId()));
    mesh_values.push_back(String(mesh.getNodeId()));
}

//this function will delete the active mesh nodes before inserting the new mesh network active nodes in
void MeshBridge::firestoreMeshCollectionClear(){
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
    }
        String documentPath = "MeshNetwork/active";
        FirebaseJson content;
    if (Firebase.Firestore.deleteDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
    {
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        Serial.print(fbdo.payload());
        return;
    }else{
      Serial.println(fbdo.errorReason());
    }
    }

void MeshBridge:: firestoreReadChanges()
{
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
        for(list<String>::iterator iter= mesh_values.begin(); iter!=mesh_values.end() ; ++iter ){
            String documentPath = "/Changes/"+ (*iter);
            FirebaseJson content;
            bool response;
            Serial.printf("check change for %s\n",documentPath.c_str());
        //if we get a document here hence the node_id has gone through changes.
        if(Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
            //now we will scan the Nodes collection in order to retrieve all changes: 
            Serial.printf("recieved %s\n", fbdo.payload().c_str());
            String changeid;
            String network_data;
            if( get_node_changes((*iter), changeid) && firestoreReadNetwork(network_data))
                change_log[(*iter)] = changeid + network_data;
            Serial.println("done retrieving:\n\n");
            Serial.println(change_log[(*iter)]);   
        }
        // A CODE TO DELETE A CHANGE THAT HAS BEEN READ:: for now its in comment cause the insertion is manual
        // if (Firebase.Firestore.deleteDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
        // {
        //     Serial.printf("deleted \n%s\n\n", fbdo.payload().c_str());
        //     return;
        // }else{
        // Serial.println(fbdo.errorReason());
        // }
        }

    }
}

bool MeshBridge::get_node_changes(String node_id, String &changes)
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        String document_path = "Changes/" + node_id;
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
            if(!Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str())){
                break;
            }
            // succeed = json.setJsonData(fbdo.payload()) && json.get(data, "fields") && data.getJSON(json);
            Serial.println(succeed);
            if(!json.setJsonData(fbdo.payload())){
                Serial.println("set json fail");
                continue;
            }
            succeed = json.setJsonData(fbdo.payload());
            Serial.println(succeed);
            succeed =  json.get(data, "fields") ;
            Serial.println(succeed);
            succeed =  data.getJSON(json);
            Serial.println(succeed);
            //Serial.println(json.raw());
            if(succeed){
                changes = json.raw();
                return true;
            }
        } while (!succeed && response_size < 16384); // TODO consider this number again
    }
    Serial.println("data failed");
    Serial.println(fbdo.errorReason());
    return false;
}

void MeshBridge::firestoreNetworkDataCollectionUpdate(){

    if (WiFi.status() == WL_CONNECTED && Firebase.ready()){
        String documentPathNetwork = "Network/mesh";
        FirebaseJson content_m;
        bool response2;
        String prefix = MESH_PREFIX;
        String password = MESH_PASSWORD;
        int port = MESH_PORT; 
        content_m.set("fields/mesh_prefix/stringValue/",prefix.c_str());
        content_m.set("fields/mesh_password/stringValue/",password.c_str());
        content_m.set("fields/mesh_port/integerValue/",port);

        if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPathNetwork.c_str(), content_m.raw())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        }else{
            Serial.println(fbdo.errorReason());
        }
    }
}

bool MeshBridge::firestoreReadNetwork(String &changes){
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
            if(!Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str())){
                break;
            }
            // succeed = json.setJsonData(fbdo.payload()) && json.get(data, "fields") && data.getJSON(json);
            Serial.println(succeed);
            if(!json.setJsonData(fbdo.payload())){
                Serial.println("set json fail");
                continue;
            }
            succeed = json.setJsonData(fbdo.payload());
            Serial.println(succeed);
            succeed =  json.get(data, "fields") ;
            Serial.println(succeed);
            succeed =  data.getJSON(json);
            Serial.println(succeed);
            //Serial.println(json.raw());
            if(succeed){
                changes = json.raw();
                return true;
            }
        } while (!succeed && response_size < 16384); // TODO consider this number again
    }
    Serial.println("data failed");
    Serial.println(fbdo.errorReason());
    return false;
    }

void MeshBridge::set_bridge_in_firebase(String nodeId){
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
        //part1: initializing empty Node in the node list dedicated for the bridge
        String documentPath = "Nodes/"+nodeId;
        FirebaseJson content;
        bool response;
        content.set("fields/nickname/stringValue/",nodeId.c_str());
        content.set("fields/bridge/booleanValue/",true);
        content.set("fields/active/booleanValue/",true);
        content.set("fields/configured/booleanValue/",false);
        if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            Serial.printf("initializing bridge succeed!!!\n\n");
            initialized=true;
            return;
        }
        else{
            Serial.println(fbdo.errorReason());
        }
        //part2: initializing empty Node in the nickname list dedicated for the node
        String documentPath2 = "NodesNickname/"+nodeId;
        FirebaseJson content2;
        bool response2;
        content2.set("fields/nodeId/stringValue/",nodeId.c_str());

        if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath2.c_str(), content2.raw())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        }
        else{
            Serial.println(fbdo.errorReason());
        }

    }
}

void MeshBridge::set_in_firebase(String nodeId){
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
        //part1: initializing empty Node in the node list dedicated for the bridge
        Serial.println("initializing Node" + nodeId);
        String documentPath = "Nodes/"+nodeId;
        FirebaseJson content;
        bool response;
        content.set("fields/nickname/stringValue/",nodeId.c_str());
        content.set("fields/bridge/booleanValue/",false);
        content.set("fields/active/booleanValue/",true);
        content.set("fields/configured/booleanValue/",false);
        if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            Serial.printf("initializing bridge succeed!!!\n\n");
            initialized=true;
            set_default_nickname(nodeId);
            return;
        }
        else{
            Serial.println(fbdo.errorReason());
            Serial.printf("initializing Node failed!!!\n\n");
        }
    }
}

void MeshBridge::set_default_nickname(String nodeId){
    if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
        String documentPath = "NodesNickname/"+ nodeId;
        FirebaseJson content;
        bool response;
        content.set("fields/nodeId/stringValue/",nodeId.c_str());

        if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        }
        else{
            Serial.println(fbdo.errorReason());
        }
    }
}