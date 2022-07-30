#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>
#include <vector>
#include <functional>
#include "Device.h"
#include "constants_utils.h"

#define DEVICE_TYPE_SENSOR F("Sensor")
#define MEASUREMENT_TYPE_KEY F("measuremen_type")
#define UNITS_KEY F("units")

struct Measurement
{
    unsigned int sensor_id;
    String type;
    float value;
    Time time;
};

typedef std::vector<Measurement> Measurements;
typedef std::function<Measurements()> measure_callback_t;
// typedef std::function<Measurement()> get_values_callback_t;

class Sensor : public Device
{

protected:
    // TODO config this at calibration
    bool calibrated = true;
    Sensor() = delete;
    Sensor(DEVICE_CONSTRUCTOR_ARGUMENTS);
    // Sensor(int id, String hardware_info, uint8_t data_pins, String measurement_type, String units, uint8_t power_pin = -1);
    // Sensor(int id, String hardware_info, uint8_t data_pins, String measurement_type[], String units[], uint8_t power_pin = -1);
    // Sensor(int id, String hardware_info, uint8_t data_pins, std::vector<String> measurement_type, std::vector<String> units, uint8_t power_pin = -1);

    static std::vector<String> from_envelop(DynamicJsonDocument envelop, String key);

public:
    const std::vector<String> MEASUREMENTS_TYPE; // identifier for what is being measured. e.g. humidity, temperature // TODO should be array
    const std::vector<String> UNITS;             // the units of the measurements. e.g. voltage, percentage, Celsius // TODO should be array

    Measurements measure_wrapper();
    virtual Measurements measure() = 0;
    // virtual void measure() = 0;
    // virtual Measurement get_values() = 0;
    virtual void calibrate() = 0;
    virtual measure_callback_t get_measure_callback();
    // virtual get_values_callback_t get_values_callback();
};

#endif /* _SENSOR_H_ */