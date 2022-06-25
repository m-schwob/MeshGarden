#include <Arduino.h>
#include <EEPROM.h> //https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/EEPROM.h

struct Time
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
};

#define EEPROM_SIZE (sizeof(Time) + sizeof(int))

void store_time(Time &time, int &sleep_time)
{
    EEPROM.put(0, sleep_time);
    EEPROM.put(sizeof(sleep_time), time);
    EEPROM.commit();
    Serial.println("time stored");
}

void load_time(Time &time, int &sleep_time)
{
    EEPROM.get(0, sleep_time);
    EEPROM.get(sizeof(sleep_time), time);
    Serial.println("time loaded");
}

void setup()
{
    // Init Serial USB
    Serial.begin(115200);
    Serial.println();
    Serial.println(F("Initialize System"));

    // Init EEPROM
    EEPROM.begin(EEPROM_SIZE);

    Time time1;
    int sleep_time1 = 0;

    // Read data from eeprom
    load_time(time1, sleep_time1);

    Serial.println(time1.hours);
    Serial.println(time1.minutes);
    Serial.println(time1.seconds);
    Serial.println(sleep_time1);
    EEPROM.end();



    EEPROM.begin(EEPROM_SIZE);
    Time time;
    time.hours = 1;
    time.minutes = 15;
    time.seconds = 25;
    int sleep_time = 40;

    // Write data into eepromF
    store_time(time, sleep_time);

    Serial.println(time.hours);
    Serial.println(time.minutes);
    Serial.println(time.seconds);
    Serial.println(sleep_time);

    EEPROM.end();
}

void loop() {}