#include "device.h"

Device::Device(DEVICE_CONSTRUCTOR_ARGUMENTS)
    : DEVICE_ID(device_id), DEVICE_TYPE(envelop[DEVICE_TYPE_KEY].as<String>()),
      HARDWARE_INFO(hardware_info), PINOUT(pinout), POWER_PIN_CONTROL(pinout.containsKey(POWER_PIN))
{
    if (POWER_PIN_CONTROL)
    {
        pinMode(POWER_PIN_CONTROL, OUTPUT);
        digitalWrite(POWER_PIN_CONTROL, LOW);
    }
    Serial.printf("%s: initalized\n", HARDWARE_INFO.c_str());
}

DynamicJsonDocument Device::create_envelop(String device_type)
{
    DynamicJsonDocument doc(2 * (JSON_OBJECT_SIZE(1) + device_type.length() + 1)); // TODO how to calculate it right? maybe needs to make the key const*
    // Serial.println(device_type + " " + String(device_type.length()) + " "+ String(doc.capacity()));
    doc[DEVICE_TYPE_KEY] = device_type;
    if (doc.overflowed())
        Serial.println("ERROR: device_type string overflow"); // think how to handle this case. exit? just log?
    doc.shrinkToFit();
    return doc;
}

// Device::Device(int id, String device_type, String hardware_info, uint8_t data_pins, uint8_t power_pin)
//         : Device(id, std::vector<String>({device_type}), hardware_info, DynamicJsonDocument pinout){}

// Device::Device(int id, String device_type[], String hardware_info, uint8_t data_pins, uint8_t power_pin)
//         : Device(id, std::vector<String>(device_type, arr_end(device_type)), hardware_info, DynamicJsonDocument pinout){}

// Device::Device(int id, std::vector<String> device_type, String hardware_info, DynamicJsonDocument pinout)
//         : ID(id), DEVICE_TYPE(device_type), HARDWARE_INFO(hardware_info), PINOUT(pinout) {
//     if(POWER_PIN_CONTROL != -1){
//         pinMode(POWER_PIN_CONTROL, OUTPUT);
//         digitalWrite(POWER_PIN_CONTROL, LOW);
//     }
// }

void Device::power_on()
{
    if (POWER_PIN_CONTROL)
        digitalWrite(PINOUT[POWER_PIN], HIGH);
}
void Device::power_off()
{
    if (POWER_PIN_CONTROL)
        digitalWrite(PINOUT[POWER_PIN], LOW);
}

// by default enabling the device does not power it on.
void Device::enable(bool _power_on)
{
    enabled = true;
    if (_power_on && POWER_PIN_CONTROL)
        power_on();
}

// disabling the device will allways power it off
void Device::disable()
{
    enabled = false;
    power_off();
}