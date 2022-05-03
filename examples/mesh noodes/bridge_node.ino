#include <Arduino.h>
#include <PubSubClient.h>
#include <map>
#include <painlessMesh.h>

using namespace painlessmesh;

/*******************************************************************************
 *  Macro Definitions
 ******************************************************************************/
#define MQTT_BROKER_IP                  192, 168, 1, 111
#define MQTT_BROKER_PORT                1883
#define MQTT_CLIENT_ID                  "my-client"
#define MQTT_USER                       "myUser"
#define MQTT_PASS                       "myPass"
#define MQTT_CHECK_CONNECTION_S         60

#define CFG_MESH_PREFIX               	"MyMesh"
#define CFG_MESH_PASSWORD             	"MyMesh1234"
#define CFG_MESH_PORT                 	5555
#define CFG_MESH_CHANNEL                11

#define CFG_MESH_PACKET_TYPE_REQUEST    "request"
#define CFG_MESH_PACKET_TYPE_SENSOR     "sensor"

#define CFG_STATION_SSID          		"StationId"
#define CFG_STATION_PASSWORD      		"StationPass"
#define CFG_HOSTNAME              		"MyMesh_Bridge"

#define CFG_TOPIC_ROOT            		"MyRootTopic/"
#define CFG_TOPIC_SYSTEM_IN       		"MyRootTopic/system/to/service"
#define CFG_TOPIC_SYSTEM_OUT      		"MyRootTopic/system/from/service"
#define CFG_TOPIC_CAMS_IN         		"MyRootTopic/sensors/cams/to/"
#define CFG_TOPIC_CAMS_OUT        		"MyRootTopic/sensors/cams/from/"

#define NODE_ID              			"BRIDGE"

#define TIMING_INTERVAL_S               10


/*******************************************************************************
 *  NAMED MESH
 ******************************************************************************/
typedef std::function< void( String &from, String &msg ) > namedReceivedCallback_t;

class namedMesh : public painlessMesh
{
	public:

		namedMesh()
		{
			auto cb = [ this ]( uint32_t from, String &msg )
			{
				// Try to parse it.. Need to test it with non json function
				DynamicJsonDocument jsonBuffer( 1024 + msg.length() );
				DeserializationError error = deserializeJson( jsonBuffer, msg );
				if ( error )
				{
					Serial.print( F( "DeserializeJson() failed: " ) );
					Serial.println( error.c_str() );
					return;
				}
				JsonObject root = jsonBuffer.as< JsonObject >();
				if ( root.containsKey( "topic" ) && String( "nameBroadCast" ).equals( root[ "topic" ].as< String >() ) )
				{
					nameMap[ from ] = root[ "name" ].as< String >();
				}
				else
				{
					if ( userReceivedCallback )
					{
						// If needed send it on to userReceivedCallback
						userReceivedCallback( from, msg );
					}

					if ( userNamedReceivedCallback )
					{
						String name;
						// If needed look up name and send it on to
						// userNamedReceivedCallback
						if ( nameMap.count( from ) > 0 )
						{
							name = nameMap[ from ];
						}
						else
						{
							name = String( from );
						}

						userNamedReceivedCallback( name, msg );
					}
				}
			};

			painlessMesh::onReceive( cb );

			changedConnectionCallbacks.push_back( [ this ]( uint32_t id )
			{
				if ( nameBroadCastTask.isEnabled() )
				nameBroadCastTask.forceNextIteration();
			} );
		}

		bool checkNode( String &name )
		{
			for ( auto && pr : nameMap )
			{
				if ( name.equals( pr.second ) )
				{
					if ( this->isConnected( pr.first ) )
					{
						return true;
					}
				}
			}

			return false;
		}

		void printMap( void )
		{
			Serial.println( "Name Map:" );

			for ( auto && pr : nameMap )
			{
				Serial.println( "( " + String ( pr.first ) + " ; " + pr.second + " )" );
			}
		}

		String getNodeName( uint32_t nodeId )
		{
			// Look up name
			for ( auto && pr : nameMap )
			{
				if ( nodeId == pr.first )
				{
					String name = pr.second;
					return name;
				}
			}

			return String( "NULL" );
		}

		String getLocalName( void )
		{
			return nodeName;
		}

		void setName( String &name )
		{
			nodeName = name;
			// Start broadcast task if not done yet
			if ( !nameBroadCastInit )
			{
				// Initialize
				nameBroadCastTask.set( 5 * TASK_MINUTE, TASK_FOREVER, [ this ]()
				{
					String msg;
					// Create arduinoJson msg
					DynamicJsonDocument jsonBuffer( 1024 );
					JsonObject root = jsonBuffer.to< JsonObject >();
					root[ "topic" ] = "nameBroadCast";
					root[ "name" ] = this->getName();
					serializeJson( root, msg );
					this->sendBroadcast( msg );
				} );

				// Add it
				mScheduler->addTask( nameBroadCastTask );
				nameBroadCastTask.enableDelayed();

				nameBroadCastInit = true;
			}
			nameBroadCastTask.forceNextIteration();
		}

		using painlessMesh::sendSingle;
		bool sendSingle( String &name, String &msg )
		{
			// Look up name
			for ( auto && pr : nameMap )
			{
				if ( name.equals( pr.second ) )
				{
					uint32_t to = pr.first;
					return painlessMesh::sendSingle( to, msg );
				}
			}

			return false;
		}

		virtual void stop()
		{
			nameBroadCastTask.disable();
			mScheduler->deleteTask( nameBroadCastTask );
			painlessMesh::stop();
		}

		virtual void onReceive( receivedCallback_t onReceive )
		{
			userReceivedCallback = onReceive;
		}

		void onReceive( namedReceivedCallback_t  onReceive )
		{
			userNamedReceivedCallback = onReceive;
		}

	protected:

		String nodeName;
		std::map< uint32_t, String > nameMap;

		receivedCallback_t              userReceivedCallback;
		namedReceivedCallback_t         userNamedReceivedCallback;

		bool nameBroadCastInit = false;
		Task nameBroadCastTask;
};

/*******************************************************************************
 *  Global Variables
 ******************************************************************************/
IPAddress myIP( 0, 0, 0, 0 );
IPAddress mqttBroker( MQTT_BROKER_IP );
namedMesh  mesh;
WiFiClient wifiClient;
PubSubClient mqttClient;
bool mqttClientConnected = false;
String nodeName = NODE_ID;
uint32_t nexttime = 0;
bool initialized = false;

/*******************************************************************************
 *  Functions Definitions
 ******************************************************************************/
void namedReceivedCallback( String &from, String &msgFrom )
{
	DynamicJsonDocument jsonBufferFrom( 1024 + msgFrom.length() );
	DeserializationError error = deserializeJson( jsonBufferFrom, msgFrom );
	if ( error )
	{
		Serial.print( F( "DeserializeJson() failed: " ) );
      	Serial.println( error.c_str() );
		return;
	}
	JsonObject rootFrom = jsonBufferFrom.as< JsonObject >();

	if ( rootFrom.containsKey( "type" ) && String( "request" ).equals( rootFrom[ "type" ].as< String >() ) )
	{
		String request = rootFrom[ "packet" ].as< String >();

		if ( String( "MQTT ready?" ).equals( request ) )
		{
			String response = mqttClientConnected ? "true" : "false";
			String msgTo;
			String to = from;

			DynamicJsonDocument jsonBufferTo( 1024 );
			JsonObject rootTo = jsonBufferTo.to< JsonObject >();
			rootTo[ "nodeId" ] = nodeName;
			rootTo[ "type" ] = CFG_MESH_PACKET_TYPE_REQUEST;
			rootTo[ "packet" ] = String( nodeName + " ready " + response );
			serializeJson( rootTo, msgTo );
			Serial.println( "<--- Received " + String( CFG_MESH_PACKET_TYPE_REQUEST ) + " from " + from );
			mesh.sendSingle( to, msgTo ); 
		}
	}
	else if ( rootFrom.containsKey( "type" ) && String( "sensor" ).equals( rootFrom[ "type" ].as< String >() ) )
	{
		if ( mqttClientConnected == true )
		{
			String msgTo;

			DynamicJsonDocument jsonBufferTo( 1024 + msgFrom.length() );
			JsonObject rootTo = jsonBufferTo.to< JsonObject >();
			rootTo[ "nodeId" ] = from;
			rootTo[ "sensor" ] = rootFrom[ "packet" ][ "sensor" ].as< String >();
			rootTo[ "idPacket" ] = rootFrom[ "packet" ][ "idPacket" ].as< String >();
			rootTo[ "indexPacket" ] = rootFrom[ "packet" ][ "indexPacket" ].as< String >();
			rootTo[ "totalPackets" ] = rootFrom[ "packet" ][ "totalPackets" ].as< String >();
			rootTo[ "data" ] = rootFrom[ "packet" ][ "data" ].as< String >();
			serializeJson( rootTo, msgTo );

			Serial.println( msgTo );
			Serial.println( "<--- Received " + rootFrom[ "packet" ][ "sensor" ].as< String >() + " from " + from );
			String topic = TOPIC_CAMS_OUT + String( from );
			mqttClient.publish( topic.c_str(), msgTo.c_str() );
		}
	}
}


void droppedConnectionCallback( uint32_t nodeId )
{
	String name = mesh.getNodeName( nodeId );
	Serial.println( "Dropped connection with " + name );
}


void newConnectionCallback( uint32_t nodeId )
{
	String name = mesh.getNodeName( nodeId );
	Serial.println( "New connection with " + name );
}


void changedConnectionsCallback( void )
{
	Serial.println( "Layout of the mesh changed" );
}


void mqttCallback( char* topic, uint8_t* payload, unsigned int length )
{
	char* cleanPayload = ( char* )malloc( length + 1 );
	memcpy( cleanPayload, payload, length );
	cleanPayload[ length ] = '\0';
	String msg = String( cleanPayload );
	free( cleanPayload );

	String targetStr = String( topic ).substring( 8 );

	if ( targetStr == "system/to/service" )
	{
		if ( msg == "getNodes" )
		{
			auto nodes = mesh.getNodeList( true );
			String str;

			for ( auto && id : nodes )
			{
				str += String( id ) + String( " " );
			}        

			mqttClient.publish( TOPIC_SYSTEM_OUT, str.c_str() );
		}
		else
		{
			mqttClient.publish( TOPIC_SYSTEM_OUT, "Service message unknown" );
		}
	}
	else
	{
		mqttClient.publish( TOPIC_SYSTEM_OUT, "Wrong topic" );
	}
}


void mqttConnectionHandle( void ) 
{
	while ( !mqttClient.connected() ) 
	{
		Serial.println( "Attempting MQTT connection..." );  
		
		if ( mqttClient.connect( MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS ) && mqttClient.setBufferSize( 16384 ) ) 
		{
			Serial.println( "Connected to MQTT broker" );  
			mqttClient.publish( TOPIC_SYSTEM_OUT, "Ready!" );
			mqttClient.subscribe( TOPIC_SYSTEM_IN );
			mqttClientConnected = true;
		}
		else
		{
			Serial.print( "Failed, rc=" );
			Serial.print( mqttClient.state() );
			Serial.println( " try again in 2 seconds" );
			delay( 2000 );
			mesh.update();
			mqttClient.loop();
		}
	}
}


/*******************************************************************************
 *******************************************************************************
 *
 *  SETUP FUNCTION
 *
 *******************************************************************************
 ******************************************************************************/
void setup()
{
	Serial.begin( 115200 );

	// MESH SETUP
	mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );
	mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, CFG_MESH_CHANNEL );
	mesh.setName( nodeName );
	mesh.onReceive( &namedReceivedCallback );
	mesh.onDroppedConnection( &droppedConnectionCallback );
	mesh.onNewConnection( &newConnectionCallback );
	mesh.onChangedConnections( &changedConnectionsCallback );
	
//connecting to the new AP station:
	mesh.stationManual( STATION_SSID, STATION_PASSWORD );
	mesh.setHostname( HOSTNAME );
	mesh.setRoot( true );
	mesh.setContainsRoot( true );

//	
	mesh.initOTAReceive( NODE_ID );
	mqttClient.setServer( mqttBroker, MQTT_BROKER_PORT );
	mqttClient.setCallback( mqttCallback );  
	mqttClient.setClient( wifiClient );

	Serial.println( "\n********** Hi! I'm the " + String( NODE_ID ) + " **********\n" );
}

/*******************************************************************************
 *******************************************************************************
 *
 *  LOOP FUNCTION
 *
 *******************************************************************************
 ******************************************************************************/
void loop()
{
	mesh.update();
	mqttClient.loop();

//initialize the IP address
	if ( myIP != getlocalIP() )
	{
		myIP = getlocalIP();
		Serial.println( "My IP is " + myIP.toString() );
		initialized = true;
	}

//if time interval is initialized and we are at the next schedualled time for checking connection, we will check
	if ( ( millis() >= nexttime ) && ( initialized ) )
    {
		nexttime = millis() + MQTT_CHECK_CONNECTION_S * 1000;

		if ( !mqttClient.connected() ) 
		{
			mqttClientConnected = false;
			mqttConnectionHandle();
		}
    }
}
