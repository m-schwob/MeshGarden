#include <Arduino.h>
#include <LittleFS.h>
#include <Firebase_ESP_Client.h>

template <typename T>
bool match_path(String &path)
{
	if (std::is_same<T, String>::value)
		path = path + "/stringValue";
	else if (std::is_same<T, int>::value)
		path = path + "/integerValue";
	else if (std::is_same<T, FirebaseJsonArray>::value)
		path = path + "/arrayValue/values";
	else if (std::is_same<T, FirebaseJson>::value)
		path = path + "/mapValue/fields";
	else
		return false;
	return true;
}

template <typename T>
bool firestore_get(FirebaseJson &json, FirebaseJsonData &data, String &path, T &result)
{
	if (!path.isEmpty() && !json.isMember(path))
	{
		Serial.println(F("invalid path"));
		return false;
	}

	if (!match_path<T>(path))
	{
		Serial.println("unsupported type");
		return false;
	}

	if (!json.get(data, path))
	{
		Serial.println(F("unexpected type"));
		return false;
	}
	return true;
}

template <typename T>
bool firestore_get(FirebaseJson &json, String path, T &result)
{
	FirebaseJsonData data;
	if (!firestore_get(json, data, path, result))
		return false;
	data.get<T>(result);
	return true;
}

template <typename T>
bool firestore_value(FirebaseJson &json, String path, T &result)
{
	FirebaseJsonData data;
	if (!firestore_get<T>(json, data, path, result))
		return false;
	result = data.to<T>();
	return true;
}

// get array and index and return the json object of that index
bool firestore_array(FirebaseJsonArray &arr, int index, FirebaseJson &result)
{
	FirebaseJsonData data;	
	arr.get(data, index);
	data.getJSON(result);
	return true;
}

// get array and index and return json  or array
template <typename T>
bool firestore_array_get(FirebaseJsonArray &arr, int index, T &result)
{
	FirebaseJson json;
	firestore_array(arr, index, json);
	firestore_get(json, "", result);
	return true;
}

// get array and index and return value
template <typename T>
bool firestore_array_value(FirebaseJsonArray &arr, int index, T &result)
{
	FirebaseJson json;
	firestore_array(arr, index, json);
	firestore_value(json, "", result);
	return true;
}

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

	// read pre uploaded file to string
	Serial.println("read pre uploaded configuration file");
	File pre_file = LittleFS.open("/config.json", "r");
	String config = pre_file.readString();
	// delay(1);  check if helping in case of fails
	pre_file.close();
	Serial.print(F("size: "));
	Serial.println(config.length());
	Serial.println();

	// saving file
	Serial.println("saving configuration to file");
	File file = LittleFS.open("/config.json", "w");
	size_t size = file.print(config);
	// delay(1);  check if helping in case writing fails
	file.close();
	Serial.print(F("size: "));
	Serial.println(size);
	Serial.println();

	// read the saved file to firebase json object
	Serial.println("reading configuration file");
	FirebaseJson json;
	file = LittleFS.open("/config.json", "r");
	json.readFrom(file);

	// parse json data into variables
	String nickname;
	String firmware;
	String mesh_ssid;
	String mesh_password;
	int mesh_port = 0;
	FirebaseJsonArray sensors;

	firestore_value(json, "nickname", nickname);
	firestore_value(json, "firmware", firmware);
	firestore_value(json, "mesh_prefix", mesh_ssid);
	firestore_value(json, "mesh_password", mesh_password);
	firestore_value(json, "mesh_port", mesh_port);
	firestore_get(json, "sensors", sensors);

	Serial.println("Node Configurations:");
	Serial.print("    - "); Serial.print("Nickname: "); Serial.println(nickname);
	Serial.print("    - "); Serial.print("Firmware: "); Serial.println(firmware);
	Serial.print("    - "); Serial.println("Mesh Network: ");
	Serial.print("        - "); Serial.print("SSID: "); Serial.println(mesh_ssid);
	Serial.print("        - "); Serial.print("Password: "); Serial.println(mesh_password);
	Serial.print("        - "); Serial.print("Port: "); Serial.println(mesh_port);
	Serial.print("    - "); Serial.println("Sensors: ");

	for (size_t i = 0; i < sensors.size(); i++){
		FirebaseJson sensor;
		firestore_array_get(sensors, i, sensor);

		FirebaseJson pinout;
		FirebaseJsonArray sensor_type;
		int sensor_id;
		int sample_interval;
		FirebaseJsonArray units;
		String hardware_info;

		firestore_value(sensor, "hardware_info", hardware_info);
		firestore_value(sensor, "sensor_id", sensor_id);
		Serial.print("        - "); Serial.print(hardware_info); Serial.println(":");
		Serial.print("            - "); Serial.print("Id: "); Serial.println(sensor_id);

		firestore_get(sensor, "sensor_type", sensor_type);
		firestore_get(sensor, "units", units);
		Serial.print("            - "); Serial.print("Types[units]: ");
		for (size_t i = 0; i < sensor_type.size(); i++){
			String type;
			String unit;
			firestore_array_value(sensor_type, i, type);
			firestore_array_value(units, i, unit);
			Serial.print(type); Serial.print("["); Serial.print(unit); Serial.print("], ");
		}
		Serial.println();

		firestore_get(sensor, "pinout", pinout);
		Serial.print("            - "); Serial.print("Pinout: ");
		for (size_t i = 0; i < pinout.iteratorBegin(); i++){
			FirebaseJson::IteratorValue value = pinout.valueAt(i);
                Serial_Printf("Name%u: %s, Value: %s, Type: %s\n", value.depth, value.key.c_str(), value.value.c_str(), value.type == FirebaseJson::JSON_OBJECT ? "object" : "array");
		}
		Serial.println();
		
		firestore_value(sensor, "sample_interval", sample_interval);
		Serial.print("            - "); Serial.print("Samples Interval: "); Serial.println(sample_interval);
	}
}

void loop()
{
	delay(5000);
}