#ifndef _UVI_SENSOR_H_
#define _UVI_SENSOR_H_

#include <SparkFun_VEML6075_Arduino_Library.h>
#include "constants_utils.h"
#include "pins_maps.h"
#include "Device.h"
#include "Sensor.h"

#define UVI_KEY "UVi"

class UViSensor : public Sensor
{
public:
    static const String HARDWARE_INFO;

private:
    VEML6075 uv;

public:
    UViSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

REGISTER_DEVICE(UViSensor, UViSensor::HARDWARE_INFO);

#endif /* _UVI_SENSOR_H_ */