#ifndef _DUMMYSENSOR_H_
#define _DUMMYSENSOR_H_

#include "Sensor.h"

#define _HARDWARE_INFO "Dummy Sensor"


class DummySensor : public Sensor{
    private:
        float result;
    public:
        DummySensor(int id, String sensor_type, String measurement_type, String units, uint8_t power_pin = -1);
        void measure();
        Measurement get_values();
        void calibrate();
};

#endif /* _DUMMYSENSOR_H_ */