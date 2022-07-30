# MeshGarden - introduction

MeshGarden project is an iot system designed to monitor and maintain small and large scale gardens the system supports a verity of controllers. 
For now its designed to work on ESP8266 and ESP32
the system requires only one ESP32 micro controller to be connected to the Wi-Fi (bridge node) and can maintain many ESP8266 nodes that will take measurements 
from easy to custome sensors.
messages passes down from the sensors to the bridge node via local Mesh network,
the data passes from the bridge node to a firestore server and can be accessed and changed with a designated cell phone app.

# MeshGarden - system diagram
![Alt text](/docs/images/system_scheme.jpeg)

As explained, the system composed over 3 major parts: the mesh network , the firestore server and the MeshGarden application 
that are interconnected.

# MeshGarden - library dependency

* painlessmesh/painlessMesh@^1.5.0

* mobizt/Firebase Arduino Client Library for ESP8266 and ESP32 @ ^4.0.3

* bblanchon/ArduinoJson @ ^6.19.4

* adafruit/DHT sensor library@^1.4.3

* adafruit/Adafruit ADS1X15@^2.4.0

* me-no-dev/ESP Async WebServer @ 1.2.3

* fbiego/ESP32Time@^1.1.0

# MeshGarden - features
Mesh network features:

* Mesh networks allows the user to disperse the sensors over a larger area with no need to be in proximity to Wi-Fi AP
* The system allows the user to add its own costumize sensors easily by writing his own 2 functions
* Mesh networks creates stability, it has the ability to fix itself when a node is down  

![Alt text](/docs/images/plant_disperse.png)

Application features:

* Shows the active nodes in the network
* Change node nicknames of nodes in order to identify where they are located
* Configure new sensors to a node
* Presents the user the measurements of a selected node
* Present the user time series graph of the measures taken by a sensor
* Present the active nodes and their battery values to the user  

App screenshots:



## **step 1: ** setup firebase server

follow the instruction in the videos bellow by "That Project" in order to setup your own firestore server

video 1:
set a firebase server https://www.youtube.com/watch?v=KV0D8nrsBLg&t=0s  
*TIP* : when choosing the cloud firestore location use the https://www.gcping.com 
site in order to find the server that provide the minimal latency 


video 2:
connect the firebase to the app https://www.youtube.com/watch?v=nsopdabOcug&t=3s   
*Note* : The files provided in the MeshGaredApp directory are source code *only*, so in order to use them,
create a new flutter project and follow the above video  


## **step 2:** create an account in the MeshGarden app with your own usermane and password

## **step 3:** bui 

## **step 4:** change the configue.json to fit your own project.

config json for example

![Alt text](/docs/images/config_json.png)

user_email,user_password - the username and password with whom you registered to the application
api_key - the unique key generated for your firebase server
firebase_project_id - the firebase progect id 

**mesh_prefix** - the SSID of the mesh network

**mesh_password** - the Password for the mesh network

**mesh_port** - a port the mesh network will run with

**mesh_connection_time** - the number of seconds within them the mesh network will run and be active (for battery saving for the nodes)

**deep_sleep_time** - the number of seconds withing them all the sensor nodes will go to deep sleep, in that time the bridge connects to the  internet and updates/read the changes from the database


**gmt_offset_sec,ntp_server,daylight_offset_sec**- values set in order to read the current time and make sure the nodes will store the current time in the user region/country


**ssid,password** - the local Wi-Fi ssid and password. Connects the bridge to the internet 

# MeshGarden - add new sensor guide

* build a new ESP8266 micro controller with the new hardware for your new sensor.

* follow the instructions in the main.cpp file to create your own sensor class by programing the following functions:

* make sure to upload the code to the micro controller and connect it to the mesh network

* enter the MeshGarden app find the designated node, and set the sensor as descripted in the pictures bellow

## BE AWARE: in the type value every new word has to start with a capital letter from A-Z and use underscore " _ " insted of space bar

<table>
  <tr>
    <td>enter the sensors list</td>
     <td>you can choose from prepared sensors or create your own</td>
     <td>set the pins and attributes for your sensor</td>
  </tr>
  <tr>
    <td valign="top"><img src="docs/images/sensor screen.jpg"></td>
    <td valign="top"><img src="docs/images/add sensor.jpg"></td>
    <td valign="top"><img src="docs/images/add sensor screen.jpg"></td>
  </tr>
 </table>
