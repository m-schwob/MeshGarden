#include <Arduino.h>
#include "Sensor.cpp"

// ADS1X15 related. remove later
#include <Adafruit_ADS1X15.h>

class SoilMoisureSensorGroveV1{
    private:

        uint8_t analog_pin;
        uint8_t power_pin;

        //TODO config this at calibration
        bool calibrated = true;
        float C_air = 2.2; //volts
        float C_water = 1; //volts

        float result = -1; //percentage as 0-1 value
        

        // ADS1X15 related. TODO make driver for it. check example file
        Adafruit_ADS1115 ads;

        void init_adc(){
            ads.setGain(GAIN_ONE);  
            if (!ads.begin()) {  
                Serial.println("Failed to initialize ADS.");
            }
            else
                Serial.println("ADS initialized");
        }

        float extender_measure(uint16_t pin){
            int16_t adc0 = ads.readADC_SingleEnded(pin);
            float volts0 = ads.computeVolts(adc0);
            Serial.println("extender measure"); //TODO print value 
            return volts0;
        }

        //TODO maybe move to constant file
        // return percentage, in [0,1] range, of value in range.
        // parameter min is optional and the default is 0.
        // return -1 in case of error
        float percentage(float value, float max, float min = 0){
            if(min > value || value > max) return -1;
            return (value - min)/(max - min);
        }


    public:
        String HARDWARE_INFO = "Groove Soil Moisture";
        String TYPE = "Soil Moisture";

        //TODO remove hard coded. remove default value from analog_pin after solving data saving
        SoilMoisureSensorGroveV1(uint8_t analog_pin = 0, uint8_t power_pin = -1) 
            : analog_pin(analog_pin), power_pin(power_pin){
            // init I/O power pin if define. init to power off state
            if(power_pin != -1){
                pinMode(power_pin,OUTPUT);
                digitalWrite(power_pin, HIGH); //TODO solve it and change to low
            }
            init_adc(); // ADS1X15 related.
            Serial.printf("%s: initelized", HARDWARE_INFO.c_str()); // add details about pins i.e. 
        }

        void measure(){
            // power on sensor if power pin is defined 
            if(power_pin != -1)
                digitalWrite(power_pin, HIGH);

            // TODO solve it when solving extender
            // if(!extender)
            //     analogRead(pin);
            // else
            float volt = extender_measure(analog_pin);
            //TODO handle the case that the value go off range
            result = 1 - percentage(volt, C_air, C_water);
            Serial.printf("%s: measure %f volts, %f/1 range\n", HARDWARE_INFO.c_str(), volt, result);

            // power on sensor if power pin is defined 
            if(power_pin != -1)
                digitalWrite(power_pin, HIGH); //TODO solve it and change to low
        }

        Measurement get_values(){
            Measurement measurement;
            measurement.key = TYPE;
            measurement.value = result;
            result = -1;
            return measurement;
        }
};