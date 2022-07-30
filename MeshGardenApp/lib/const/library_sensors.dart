/** Library sensors */

var DHT22_DEFAULT= {
  "hardware_info": "DHT22 Air Humidity and Temperature Sensor",
  "sample_interval": 30,
  "pinout": {"DAT": "D6"},
  "sensor_type": ["Air_Humidity", "Air_Temperature"],
  "units": ["%", "C"],
};

var SOIL_MOISTURE_DEFAULT= {
  "hardware_info": "Capacitive Soil Moisture Sensor",
  "sample_interval": 30,
  "pinout": {"AOUT": "ADS0"},
  "sensor_type": ["Soil_Moisture"],
  "units": ["%"],
};

var UV_DEFAULT= {
  "hardware_info": "UVi Sensor",
  "sample_interval": 30,
  "pinout": {},
  "sensor_type": ["UVi"],
  "units": ["UVi"],
};
