#include "library_devices/DHT22_sensor.h"

#define DHTPIN 2

DHTxxSensor::DHTxxSensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : Sensor(device_id, hardware_info, pinout, envelop)
{
    if (pinout.containsKey("DAT"))
    {
        dht = new DHT(pin(pinout["DAT"].as<String>()), DHT22);
        dht->begin();
        Serial.println(F("DHT22 initialized"));
    }
    else
    {
        Serial.print(HARDWARE_INFO.c_str());
        // TODO consider not setting default pin but disabling it
        Serial.println(":Warning: sensor pin not defined.");
    }
}

Measurements DHTxxSensor::measure()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht->readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht->readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht->readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        // return; TODO handle failed situation
    }

    // // Compute heat index in Fahrenheit (the default)
    // float hif = dht.computeHeatIndex(f, h);
    // // Compute heat index in Celsius (isFahreheit = false)
    // float hic = dht.computeHeatIndex(t, h, false);

    Measurements measurements(2);
    Measurement temperature;
    Measurement humidity;

    temperature.type = TEMPERATURE_KEY;
    temperature.value = t;
    measurements.push_back(temperature);

    humidity.type = HUMIDITY_KEY;
    humidity.value = h;
    measurements.push_back(humidity);

    return measurements;
}

void DHTxxSensor::calibrate(){ calibrated = true; }
