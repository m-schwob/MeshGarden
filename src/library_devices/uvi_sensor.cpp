#include "library_devices/uvi_sensor.h"

UViSensor::UViSensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : Sensor(device_id, hardware_info, pinout, envelop)
{
    if (uv.begin() == false)
    {
        Serial.println("Unable to communicate with VEML6075.");
    }
}
Measurements UViSensor::measure()
{
    Measurements measurements(1);
    Measurement uvi;
    uvi.type = UVI_KEY;
    uvi.value = uv.index();
    measurements.push_back(uvi);
    Serial.printf("%s: measure %f uvi\n", HARDWARE_INFO.c_str(), uvi.value);
    return measurements;
}
void UViSensor::calibrate() { calibrated = true; }