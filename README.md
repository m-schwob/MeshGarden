# MeshGarden - repository content
The repository contains 3 main folder:
- MeshGardenApp - contains the source good of the android app (built using dart).
- Firbase - contains the source code for firebase cloud functions (written in javaScript).
- ESP - contains the source code for the hardware, ESP32/ESP8266 (written in c++ using arduino framework).

Other useful folders:
- utils - contain some useful code for development but not necessary.
- ESP/examples - contains some side code and example used to develop some of the code unit saperetly. great as code reference.

# MeshGarden - introduction

MeshGarden project is an iot system designed to monitor and maintain small and large scale gardens the system supports a verity of controllers. 
For now its designed to work on ESP8266 and ESP32
the system requires only one ESP32 micro controller to be connected to the Wi-Fi (bridge node) and can maintain many ESP8266 nodes that will take measurements 
from easy to custome sensors.
messages passes down from the sensors to the bridge node via local Mesh network,
the data passes from the bridge node to a firestore server and can be accessed and changed with a designated cell phone app.

# MeshGarden - system diagram

![Alt text](/docs/images/system_diagram.png)

As explained, the system composed over 3 major parts: the mesh network , the firestore server and the MeshGarden application 
that are interconnected.

# MeshGarden - nodes life cycle

![Alt text](/docs/images/life_cycle.png)


# MeshGarden - library dependency

* painlessmesh/painlessMesh@^1.5.0

* mobizt/Firebase Arduino Client Library for ESP8266 and ESP32 @ ^4.0.3

* bblanchon/ArduinoJson @ ^6.19.4

* adafruit/DHT sensor library@^1.4.3

* adafruit/Adafruit ADS1X15@^2.4.0

* me-no-dev/ESP Async WebServer @ 1.2.3

* fbiego/ESP32Time@^1.1.0

# MeshGardenAPP - library dependency
* cupertino_icons: ^1.0.2  
* firebase_auth: ^3.1.0   
* firebase_core: ^1.6.0   
* cloud_firestore: ^3.1.14  
* google_fonts: ^2.1.0    
* adaptive_dialog: ^1.1.0  
* font_awesome_flutter: ^10.1.0  
* chart_sparkline: ^1.0.5  
* intl: ^0.17.0  
* hawk_fab_menu: ^1.1.2  
* flutter_launcher_icons: ^0.9.0  
* charts_flutter: ^0.12.0  
* syncfusion_flutter_charts: 20.1.55  
* syncfusion_flutter_core: 20.1.55  

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

 <table>
  <tr>
    <td>DHT22 Measurements</td>
     <td>UVi and Soli Moisture Measurements</td>
     <td>Time Series chart</td>
  </tr>
  <tr>
    <td valign="top"><img src="docs/images/DHT22 Measurements.jpg"></td>
    <td valign="top"><img src="docs/images/UVi and Soli Moisture Measurements.jpg"></td>
    <td valign="top"><img src="docs/images/Time Series chart.jpg"></td>
  </tr>
 </table>

# MeshGarden - setup the system

## **step 1:** setup firebase server

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

## **step 3:** assembly the micro controllers as shown bellow

## **step 4:** prepare the configure.json file

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

# MeshGarden - Hardware
## Basic Required Hardware

The system must contain at exactly one bridge (ESP32) and at least one node (ESP8266). The bridge is powered by fix power supply since its power consumption is high and can hold for long on battery. The nodes are powered by batteries and to be portable .

## Bridge Components and Assembly:
Components:
- ESP32 micro-controller module
- USB 4V 1A power adapter.
- LED + matched resistor (for example 1.8 green LED with 330ohm resistor)

> The LED use as indicator and connected in parallel to ESP32 module build in LED. So it can be skipped if the build in LED visible.

## Assembly:

![](/docs/hardware/fritzing/bridge_bb.jpg)

## Node Components and assembly:
Nodes can be assembled in many ways using any sensor. Some component are is required for the system operation and some are optional. The require components are listed in the *Basic Node Module* section below and the optional are exampled in the *Sensors Options* section.

### Basic Node Module
The below component are the base hardware of the node and required for its operations. 

Components:
- ESP8266 micro-controller module
- ADS1115 ADC expender module
- 3.7V lithium battery (2000mAh and greater is recommended)
- TP4056 lithium charging module
- MCP1700-3302E LDO 3.3V voltage regulator
- 100uF capacitor
- 1uF capacitor
- 36k resistor
- 100k resistor
- on/off switch
- NC switch or jumper 
- LED + matched resistor (for example 1.8 green LED with 330ohm resistor)

> all capacitor must be at least 5V rated.

> The LED use as indicator and connected in parallel to ESP8266 module build in LED. So it can be skipped if the build in LED visible.

Assembly:
![](/docs/hardware/fritzing/basic_node_bb.jpg)


> the on/off switch (or jumper) required for the micro-controller programming since it cannot be program while D0 and RST pin are connected.

### Sensors Options
After assembling the base node, basically any sensor can be added to the node. the library supports some sensors as build in. Using those sensor require minimal setup without writing any code. Any other sensor will require writing some code and configuration as will be explained later. 

Optional Build in Sensors:
- DHT22 humidly and temperature sensor
- Capacitive soil moisture sensor
- VEML6075 UV sensor

Optional Build in Sensors Assembly (in additional to the Base module!!):
![](/docs/hardware/fritzing/library_sensors_node_bb.jpg)


## make sure to upload the code to the micro controller and connect it to the mesh network

## enter the MeshGarden app find the designated node, and set the sensor as descripted in the pictures bellow

## Note: do not add any sensors to the bridge node 

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
