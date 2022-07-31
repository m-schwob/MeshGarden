#include <Arduino.h>
#include <Base64.h>
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include <map>
#include <painlessMesh.h>

using namespace painlessmesh;

/*******************************************************************************
 *  Macro Definitions
 ******************************************************************************/
#define CFG_MESH_PREFIX               	"MyMesh"
#define CFG_MESH_PASSWORD             	"mymesh1234"
#define CFG_MESH_PORT                 	5555
#define CFG_MESH_CHANNEL                11

#define CFG_MESH_PACKET_TYPE_REQUEST    "request"
#define CFG_MESH_PACKET_TYPE_SENSOR     "sensor"

#define CFG_MESH_MAX_PACKET_SIZE        1000

#define NODE_ID                     	"NODE01"
#define TAKE_PHOTO_INTERVAL_S       	20

#define BRIDGE_NODE_ID              	"BRIDGE"


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
Scheduler userScheduler;
namedMesh mesh;
String bridgeNodeName = BRIDGE_NODE_ID;
String nodeName = NODE_ID;
char encodedString[ 16384 ];

/*******************************************************************************
 *  Functions Definitions
 ******************************************************************************/
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


Task taskSendMessage( TASK_SECOND * TAKE_PHOTO_INTERVAL_S, TASK_FOREVER, []()
{
	if ( mesh.checkNode( bridgeNodeName ) )
	{
		Serial.println( "BRIDGE node found" );
		//Put here funcrion that starts the testing for the sensor
	}
	else
	{
		Serial.println( "BRIDGE node not found" );
	}
} );


void sendData( String sensorType, long idPacket, int indexPacket, int totalPackets, String dataSensor )
{
	String msg;
	String to = bridgeNodeName;

	DynamicJsonDocument jsonBuffer( 1024 + dataSensor.length() );
	JsonObject root = jsonBuffer.to< JsonObject >();
	root[ "nodeId" ] = nodeName;
	root[ "type" ] = CFG_MESH_PACKET_TYPE_SENSOR;
	JsonObject packet = root.createNestedObject( "packet" );
	packet[ "sensor" ] = sensorType;
	packet[ "idPacket" ] = String( idPacket );
	packet[ "indexPacket" ] = String( indexPacket );
	packet[ "totalPackets" ] = String( totalPackets );
	packet[ "data" ] = dataSensor;
	serializeJson( root, msg );

	Serial.println( msg );
	Serial.println( "---> Send " + sensorType + " packet " + String( indexPacket ) + "/" + String( totalPackets ) + " to " + to );
	mesh.sendSingle( to, msg ); 
}

//no need for that function it contained camera module details

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

	// SETUP Function for the sensor

	// MESH SETUP
	mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
	mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, CFG_MESH_CHANNEL );
	mesh.setName( nodeName );
	mesh.setRoot( false );
	mesh.setContainsRoot( true );
	mesh.onDroppedConnection( &droppedConnectionCallback );
	mesh.onNewConnection( &newConnectionCallback );
	mesh.onChangedConnections( &changedConnectionsCallback );  

	// USER TASK SETUP
	userScheduler.addTask( taskSendMessage );
	taskSendMessage.enable();

	// VARIABLES SETUP
	nodeConnected = false;
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
}
