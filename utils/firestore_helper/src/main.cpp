#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

/***************************
 *  Variables Definitions For the WiFi - Change wifi ssid,password to match yours
 **************************/
#define ssid F("My_hotspot")
#define wifi_password F("mypassword")
#define ntpServer F("pool.ntp.org")
#define gmtOffset_sec 7200
#define daylightOffset_sec 7200

/***************************
 *  Macro Definitions For the FireStore DB
 **************************/
#define API_KEY F("AIzaSyCpnmHDV7B7oR6pnKXS0VBFoqaF174UzZM")
#define FIREBASE_PROJECT_ID F("meshgarden-iot")
#define USER_EMAIL F("ioadmin@ioadmin.com")
#define USER_PASSWORD F("ioadmin")

// firebase global variables
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void heap_status()
{
	Serial.print(F("FreeHeap "));
	Serial.println(ESP.getFreeHeap());
}

// download this document from firestore and print it
void download_document(String document_path)
{
	FirebaseJson content;
	Serial.print(F("getting document: "));
	Serial.println(document_path);

	if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str()))
	{
		String pl = fbdo.payload();
		Serial.print(F("payload length:"));
		Serial.println(pl.length());
		Serial.println(F("payload:"));
		Serial.println(pl);
	}
	else
	{
		Serial.println(fbdo.errorReason());
	}
}

// upload pre loaded document to firestore
void upload_document(String file_name, String document_path)
{
	heap_status();
	Serial.println(F("delete document if exists"));
	Firebase.Firestore.deleteDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str());
	fbdo.clear();

	heap_status();
	// read pre uploaded file to string
	Serial.print(F("read pre uploaded configuration file: ")); Serial.println(file_name);
	File pre_file = LittleFS.open("/" + file_name + ".json", "r");
	String doc = pre_file.readString();
	Serial.print(F("size: ")); Serial.println(doc.length());
	pre_file.close();
	pre_file.~File();

	heap_status();
	Serial.print(F("preparing content for: ")); Serial.println(document_path);
	FirebaseJson json;
	FirebaseJson content;
	json.setJsonData(doc);
	content.set(F("fields"), json);
	Serial.print(F("content buffer size: ")); Serial.println(content.serializedBufferLength());
	doc.~String();
	json.~FirebaseJson();

	heap_status();
	Serial.print(F("create new document: ")); Serial.println(document_path);
	if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", document_path.c_str(), content.raw()))
		Serial.println(F("succeed"));
	else
		Serial.println(fbdo.errorReason());
	fbdo.clear();
}

void setup()
{
	Serial.begin(115200);
	Serial.println();

	// start file system
	if (!LittleFS.begin())
	{
		Serial.println(F("fail to start files system."));
		exit(1);
	}

	// Connect to Wi-Fi
	Serial.print(F("Connecting to "));
	Serial.println(ssid);
	WiFi.begin(ssid, wifi_password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println(F("WiFi connected."));

	// init firebase
	config.api_key = API_KEY;
	auth.user.email = USER_EMAIL;
	auth.user.password = USER_PASSWORD;
	Firebase.begin(&config, &auth);
	// fbdo.setResponseSize(1024 * 4); // minimum size is 1024 bytes

	delay(1000);

	if (WiFi.status() == WL_CONNECTED)
	{
		if (Firebase.ready())
		{
			// download
			// download_document("Measurements/nodeID");

			// since createDocument crush the controller but succeed uploading, this code assumes it will cruse and update the progress in files
			Dir root = LittleFS.openDir("/");
			while (root.next() && root.isFile())
			{
				String fullname = root.fileName();
				// assuming file extension is 3 characters
				String basename = fullname.substring(0, fullname.length() - 4);
				String extension = fullname.substring(fullname.length() - 3);
				if (extension.equals("txt"))
				{
					while (true)
					{
						File flist = LittleFS.open("/" + fullname, "r");
						String path = flist.readStringUntil('\n'); // extract 2 first line
						String firstline = flist.readStringUntil('\n');
						String fcontent = flist.readString(); // extract the rest
						// remove whitespaces
						path.trim();
						firstline.trim();
						fcontent.trim();
						flist.close();

						// rewrite the rest
						flist = LittleFS.open("/" + fullname, "w");
						// String temp = path + String("\n");
						// temp = temp + fcontent;
						flist.write((path + "\n" + fcontent).c_str());
						flist.close();

						flist.~File();
						fcontent.~String();

						// Serial.println("path");
						// Serial.println(path);
						// Serial.println("firstline");
						// Serial.println(firstline);
						// Serial.println("fcontent");
						// Serial.println(fcontent);
						
						if (firstline.equals(""))
							break;
						else
						{
							upload_document(basename, path + firstline);
						}
					}
				}
			}

			// // upload
			// String nodes[] = {"2989123787", "2989123788", "2989123789"};
			// for (unsigned int i = 0; i < sizeof(nodes) / sizeof(String); i++)
			// {
			// 	Serial.print("FreeHeap ");
			// 	Serial.println(ESP.getFreeHeap());
			// 	Serial.printf("uploading: %s\n", nodes[i].c_str());
			// 	upload_document("sample_node", nodes[i], "Nodes/");
			// 	upload_document("sample_measurement", nodes[i], "Measurements/");
			// }
		}
		else
		{
			Serial.println(F("failed to init firebase connection"));
		}
	}
	else
	{
		Serial.println(F("failed to init wifi connection"));
	}
}

void loop()
{
	delay(5000);
}