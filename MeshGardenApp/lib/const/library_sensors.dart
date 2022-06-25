/** Library sensors */

var DHT11_DEFAULT= {
  "hardware_info": "DHT11",
  "sample_interval": 30,
  "pinout": {"DAT": "D1"},
  "sensor_type": ["Air Humidity", "Air Temperature"],
  "units": ["%", "C"],
};

var SOIL_MOISTURE_DEFAULT= {
  "hardware_info": "Grove Soil Moisture Sensor v1",
  "sample_interval": 30,
  "pinout": {"AOUT": "ADS",
    "VCC": "V1"},
  "sensor_type": ["Soil Moisture"],
  "units": ["%"],
};

var UV_DEFAULT= {
  "hardware_info": "UV",
  "sample_interval": 30,
  "pinout": {"DAT": "D1",
    "VCC": "V1"},
  "sensor_type": ["Soil Moisture"],
  "units": ["%"],
};
