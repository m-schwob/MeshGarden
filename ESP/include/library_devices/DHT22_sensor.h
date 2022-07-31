#ifndef _DHTXX_H_
#define _DHTXX_H_

#include <DHT_U.h>
#include "constants_utils.h"
#include "pins_maps.h"
#include "Device.h"
#include "Sensor.h"

#define DHT22_AIR_HUMIDITY_AND_TEMPERATURE_SENSOR "DHT22 Air Humidity and Temperature Sensor"

#define TEMPERATURE_KEY "Air Temperature"
#define HUMIDITY_KEY "Air Humidity"

class DHTxxSensor : public Sensor
{

private:
    DHT *dht;

public:
    DHTxxSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

REGISTER_DEVICE(DHTxxSensor, DHT22_AIR_HUMIDITY_AND_TEMPERATURE_SENSOR);

#endif /* _DHTXX_H_ */