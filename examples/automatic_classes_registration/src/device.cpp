#include "device.h"


Device::Device(int id, String device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin)
        : Device(id, std::vector<String>({device_type}), hardware_info, data_pins, power_pin){}

Device::Device(int id, String device_type[], String hardware_info, uint8_t data_pins, uint8_t power_pin)
        : Device(id, std::vector<String>(device_type, arr_end(device_type)), hardware_info, data_pins, power_pin){}

Device::Device(int id, std::vector<String> device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin) 
        : ID(id), DEVICE_TYPE(device_type), HARDWARE_INFO(hardware_info), DATA_PINS(data_pins), POWER_PIN(power_pin) {
    if(POWER_PIN != -1){
        pinMode(POWER_PIN, OUTPUT);
        digitalWrite(POWER_PIN, LOW);
    }
}

void Device::power_on(){
    if(POWER_PIN != -1)
        digitalWrite(POWER_PIN, HIGH);
}
void Device::power_off(){
    if(POWER_PIN != -1)
        digitalWrite(POWER_PIN, LOW);
}

// by default enabling the device does not power it on. 
void Device::enable(bool _power_on){
    enabled = true;
    if(_power_on && POWER_PIN != -1)
        power_on();
}

// disabling the device will allways power it off
void Device::disable(){
    enabled = false;
    power_off();
}