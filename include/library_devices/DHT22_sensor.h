#ifndef _DHTXX_H_
#define _DHTXX_H_

#include <DHT_U.h>
#include "constants_utils.h"
#include "pins_maps.h"
#include "Device.h"
#include "Sensor.h"

#define TEMPERATURE_KEY "Temperature"
#define HUMIDITY_KEY "Humidity"

class DHTxxSensor : public Sensor
{
public:
    static const String HARDWARE_INFO;

private:
    DHT *dht;

public:
    DHTxxSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

REGISTER_DEVICE(DHTxxSensor, DHTxxSensor::HARDWARE_INFO);

#endif /* _DHTXX_H_ */