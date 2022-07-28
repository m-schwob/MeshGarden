#include <Arduino.h>
#include <ArduinoJson.h>
#include "mesh_garden.h"

#if defined(ESP8266)
ADC_MODE(ADC_VCC); // enable measure esp8266 supply input voltage
#endif

/*
This documentation will be after use for user instruction and documentation. but for now its here to simplify.

For custom sensor, user need to provide the following information:
- A function in the .ino file that know how to init the sensor (called one time when the system boot up)
- A function in the .ino file that know how to handle the measuring proccess and return the measured value in the correct units.
    - To support sensor that measure more then one value the return value must be an array. The array will use Measurement type.
- An hardware id that will also add to the app. (that will be used to init the correct sensor in run time).
- A information that will be add via the app (option for json file?):
    - hardware id - short String that uniquely represent the sensor. must matched the hardware id mentioned before.
    - pinout - the how the sensor connected to the board. for example if pin 'DAT' of the sensor connected to pin 'D1' of the
                 board, both values will linked via the app and in the above functions it will be use using pin(Sting pin_name)
                 function, for example: pinMode(pin("DAT"), INPUT), readDigital(pin("DAT")).
    - measurement_types - a list of Strings. each represent one measurement type to handle multi measurements sensors.
    - units - a list of Strings. units of each measurement_type.
    - time_interval - the time intervale between preformed measurements

For the .ino file, he idea is to give the user two way of adding his own sensor:
1. Beginners options - implementing two functions 'void init_sensor()' and 'Measurement measure()' and registering the sensor using
    provided library function 'add_sensor(String hardware_id, InitFunc init_sensor_func, MeasureFunc measure_func)'.
    (that will make a class in the background)
2. Advanced options - making a custom class inheriting the sensor class and implementing the above functions and registering the sensor
        using. This option gives user more power to override default behaviors. (consider supporting this for this project)

*/

#define BAUD_RATE 115200

/*************** CLASS EXAMPLE ****************** /
Define sensor using class example.
Capacitance soil moisture sensor using ADS1X15 ADC extender.
*/

#include <Adafruit_ADS1X15.h>
#include <SPI.h>
#include "Device.h"
#include "Sensor.h"

#define _DEVICE_TYPE "Soil Moisture Sensor"
#define _HARDWARE_INFO "Groove Soil Moisture"
#define _MEASUREMENTS_TYPE "Soil Moisture"
#define _UNITS "%"

class SoilMoistureSensorGroveV1 : public Sensor
{
private:
    uint8_t analog_pin = 0;

    float C_air = 2.2; // volts
    float C_water = 1; // volts

    // ADS1X15 related. TODO make driver for it. check example file
    Adafruit_ADS1115 _ads;

    void init_adc()
    {
        _ads.setGain(GAIN_ONE);
        if (!_ads.begin(0x4A))
        {
            Serial.println("Failed to initialize ADS.");
        }
        else
            Serial.println("ADS initialized");
    }

    float extender_measure(uint16_t pin)
    {
        int16_t adc0 = _ads.readADC_SingleEnded(pin);
        float volts0 = _ads.computeVolts(adc0);
        Serial.println("extender measure"); // TODO print value
        return volts0;
    }

    float percentage(float value, float max, float min = 0)
    {
        if (min > value || value > max)
            return -1;
        return (value - min) / (max - min);
    }

public:
    SoilMoistureSensorGroveV1(DEVICE_CONSTRUCTOR_ARGUMENTS)
        : Sensor(device_id, hardware_info, pinout, envelop)
    {
        init_adc();                                               // ADS1X15 related.
        Serial.printf("%s: initalized\n", HARDWARE_INFO.c_str()); // add details about pins i.e.
    }

    Measurements measure()
    {
        Measurements measurements(1);
        Measurement moisture;
        moisture.type = "Soil Moisture";

        float volt = extender_measure(analog_pin);
        // TODO handle the case that the value go off range
        moisture.value = 1 - percentage(volt, C_air, C_water);
        measurements.push_back(moisture);
        Serial.printf("%s: measure %f volts, %f/1 range\n", HARDWARE_INFO.c_str(), volt, moisture.value);
        return measurements;
    }

    void calibrate() { calibrated = true; }
};

REGISTER_DEVICE(SoilMoistureSensorGroveV1, "Grove Soil Moisture Sensor v1");

/*****************************************************/

/****************** FUNCTIONS EXAMPLE ********************* /
Define sensor using functions example.
DHT22 air humidity and temperature sensor.
*/
#include <DHT_U.h>
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void init_DHT()
{
    dht.begin();
    Serial.println(F("DHT22 initialized"));
}

Measurements measure_DHT()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

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

    temperature.type = "Temperature";
    temperature.value = t;
    measurements.push_back(temperature);

    humidity.type = "Humidity";
    humidity.value = h;
    measurements.push_back(humidity);

    return measurements; // temporary for compiling
}

/*****************************************************/

MeshGarden garden;

REGISTER_SENSOR("DHT22 Sensor"); // TODO solve registering using add_sensor function

void setup()
{
    Serial.begin(115200);
    Serial.println();

    garden.add_sensor("DHT22 Sensor", init_DHT, measure_DHT);
    garden.begin();

    Serial.println("printing device list");
    for (String device : garden.get_device_list())
    {
        Serial.println(device);
    }
}

void loop()
{
    garden.update();
    // delay(5000); // temporary
}