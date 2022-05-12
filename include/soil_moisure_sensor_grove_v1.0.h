#ifndef _SOILMOISTURESENSORGROOVEV1_0_H_
#define _SOILMOISTURESENSORGROOVEV1_0_H_

#include <Arduino.h>
// ADS1X15 related. remove later
#include <Adafruit_ADS1X15.h>
#include "Sensor.h"

#define _HARDWARE_INFO "Groove Soil Moisture"
#define _TYPE "Soil Moisture"
#define _UNITS "%"


class SoilMoisureSensorGroveV1 : public Sensor{
    private:
        uint8_t analog_pin;

        float C_air = 2.2; //volts
        float C_water = 1; //volts

        float result = -1; //percentage as 0-1 value
        
        // ADS1X15 related. TODO make driver for it. check example file
        Adafruit_ADS1115 ads;
        void init_adc();
        float extender_measure(uint16_t pin);
        float percentage(float value, float max, float min = 0);

    public:
        SoilMoisureSensorGroveV1(uint8_t analog_pin = 0, uint8_t power_pin = -1);
        void measure();
        Measurement get_values();
        void calibrate();
};

#endif /* _SOILMOISTURESENSORGROOVEV1_0_H_ */