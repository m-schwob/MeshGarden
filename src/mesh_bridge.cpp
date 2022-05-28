#include "mesh_bridge.h"


static MeshBridge *node = NULL;

MeshBridge::MeshBridge(){
    init_mesh();
    node = this;
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
void MeshBridge::firestoreDataUpdate(String plant_id, String meas_type, String value) //TEMP parameters edited have measurement type and value 
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        Serial.printf("updating firebase with data: %s:%s\n", meas_type.c_str(), value.c_str());
        String document_path = "prototype/"; // + plant_id;
        FirebaseJson content;
        bool response;

        content.set("fields/" + plant_id + "/mapValue/fields/" + meas_type.c_str()+ "/doubleValue/", value.c_str());

        // check if node has a document nad create new if not exists
        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str(), plant_id.c_str())){
            Serial.printf("node document found\n%s\n\n", fbdo.payload().c_str());
            response = Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str(), content.raw(), "last_message");
        }
        else{
            Serial.println("node document does not exist. creating document..)");
            response = Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str(), content.raw());
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

// Needed for painless library
// receivedCallback - when we get a post on the mesh, it will update the dict by the message sent to it
// the dict will have nodeId as its key, and the Entire message as its value
void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("message received from %u msg=%s. saving to cache\n", from, msg.c_str());
    node->server_data.push_back(String(from) + "," + msg);
}

// Needed for mesh, do not change
void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", node->mesh.getNodeTime(), offset);
}

void MeshBridge::init_mesh()
{
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
   Serial.println(String(nodeId)); });
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
        // //  //pst on the firebase server what was saved on the map
        // for(std::map<String,vector<String>>::iterator iter = dict.begin(); iter != dict.end(); ++iter){
        // for(vector<String>::iterator measures_iter = iter->second.begin(); measures_iter!=iter->second.end() ; measures_iter++ ){
        // firestoreDataUpdate(iter->first, *measures_iter);
        //}
        //   }
        Serial.printf("sending %d cached messages..\n", server_data.size());
        for (vector<String>::iterator measures_iter = server_data.begin(); measures_iter != server_data.end(); measures_iter++)
        {
            Serial.printf("sending message <%s>\n", measures_iter->c_str());
            std::vector<String> vec = split((*measures_iter), ","); // split the String to vector of Strins by word
            firestoreDataUpdate(String(vec[0].c_str()), String(vec[1].c_str()), String(vec[2].c_str()));
        }
        server_data.clear();
        //  dict.clear(); //clears the map in order to be more space efficient, nodes that did not changed would not be posted again
        Serial.println("WiFi disconnected. initialize mesh");
        // initializing the mesh network again
        init_mesh();

        lasttime = millis();
    }
}