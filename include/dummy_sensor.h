#ifndef _DUMMYSENSOR_H_
#define _DUMMYSENSOR_H_

#include <vector>
#include <queue>
#include "Sensor.h"

#define _HARDWARE_INFO "Dummy Sensor"

class DummySensor : public Sensor
{
private:
    std::queue<Measurement> results;

public:
    DummySensor(int id, String sensor_type, String measurement_type, String units, uint8_t power_pin = -1);
    DummySensor(int id, String sensor_type[], String measurement_type[], String units[], uint8_t power_pin = -1);
    DummySensor(int id, std::vector<String> sensor_type, std::vector<String> measurement_type, std::vector<String> units, uint8_t power_pin = -1);
    void measure();
    Measurement get_values();
    void calibrate();
};

#endif /* _DUMMYSENSOR_H_ */