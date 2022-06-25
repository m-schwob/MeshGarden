#ifndef _BATTERY_LEVEL_SENSOR_H_
#define _BATTERY_LEVEL_SENSOR_H_

class BatteryLevelSensor : public Sensor
{
private:
    uint8_t analog_pin = 0;

    float C_air = 2.2; // volts
    float C_water = 1; // volts

    // ADS1X15 related. TODO make driver for it. check example file
    Adafruit_ADS1115 ads;

    void init_adc()
    {
        ads.setGain(GAIN_ONE);
        if (!ads.begin())
        {
            Serial.println("Failed to initialize ADS.");
        }
        else
            Serial.println("ADS initialized");
    }

    float extender_measure(uint16_t pin)
    {
        int16_t adc0 = ads.readADC_SingleEnded(pin);
        float volts0 = ads.computeVolts(adc0);
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
    BatteryLevelSensor(DEVICE_CONSTRUCTOR_ARGUMENTS)
        : Sensor(device_id, hardware_info, pinout, envelop)
    {
        init_adc();                                             // ADS1X15 related.
        Serial.printf("%s: initalized\n", HARDWARE_INFO.c_str()); // add details about pins i.e.
    }

    Measurements measure()
    {
        power_on();
        Measurements measurements(1);
        Measurement moisture;
        moisture.type = "Soil Moisture";
        
        float volt = extender_measure(analog_pin);
        // TODO handle the case that the value go off range
        moisture.value = 1 - percentage(volt, C_air, C_water);
        measurements.push_back(moisture);
        Serial.printf("%s: measure %f volts, %f/1 range\n", HARDWARE_INFO.c_str(), volt, moisture.value );
        power_off();
        return measurements;
    }

    void calibrate() { calibrated = true; }
};

// REGISTER_DEVICE(BatteryLevelSensor, "Battery Level Sensor");

#endif /* _BATTERY_LEVEL_SENSOR_H_ */