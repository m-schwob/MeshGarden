#ifndef _UVI_SENSOR_H_
#define _UVI_SENSOR_H_

#include <SparkFun_VEML6075_Arduino_Library.h>
#include "constants_utils.h"
#include "pins_maps.h"
#include "Device.h"
#include "Sensor.h"

#define UVI_SENSOR "UVi Sensor"

#define UVI_KEY "UVi"

class UViSensor : public Sensor
{

private:
    VEML6075 uv;

public:
    UViSensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    Measurements measure();
    void calibrate();
};

REGISTER_DEVICE(UViSensor, UVI_SENSOR);

#endif /* _UVI_SENSOR_H_ */