#include <Arduino.h>
#include <EEPROM.h>
#include <queue>

struct Time
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    bool Am = 1; // Am=1 refers to "Am", Am=0 refers to "Pm"
};

struct Measurement
{
    unsigned int sensor_id;
    String type;
    float value;
    Time time;
};

void print_meas(Measurement meas)
{
    // Serial.println("sensor " + String(meas.sensor_id) + " " + meas.type + " value: " + String(meas.value) +
    //                " time: " + String(meas.time.hours) + ":" + String(meas.time.minutes) + ":" + meas.time.seconds + " AM?" + meas.time.Am);
    Serial.print("sensor " + String(meas.sensor_id));
    // Serial.print(" type: " + meas.type);
    // Serial.print(" value: " + String(meas.value));
    // Serial.print(" time: " + String(meas.time.hours));
    // Serial.print(":" + String(meas.time.minutes));
    // Serial.print(":" + String(meas.time.seconds));
    // Serial.print(" AM?" + String(meas.time.Am));
    Serial.println();
}

#define MEAS_EEPROM_SIZE (sizeof(Time) + sizeof(Measurement) + 100) // 100 for string in measurement

void store_queue(int addr, std::queue<Measurement> &q)
{
    Serial.println("MEAS_EEPROM_SIZE -> " + String(MEAS_EEPROM_SIZE));
    EEPROM.begin(MEAS_EEPROM_SIZE);
    EEPROM.put(addr, q.size());
    addr += sizeof(q.size());
    EEPROM.commit();
    EEPROM.end();
    Serial.println("addr: " + String(addr));

    while (!q.empty())
    {
        Measurement meas = q.front();
        Serial.println("storing measurement at address " + String(addr));
        print_meas(meas);
        EEPROM.put(addr, meas.sensor_id);
        addr += sizeof(meas.sensor_id);
        Serial.println("addr: " + String(addr));

        // EEPROM.put(addr, meas.value);
        // addr += sizeof(meas.value);
        // EEPROM.put(addr, meas.time);
        // addr += sizeof(meas.time);

        // EEPROM.put(addr, meas.type.length()); // write the string size
        // addr += sizeof(meas.type.length());

        // write the string itself
        // for (size_t i = addr; i < addr + meas.type.length(); ++i)
        //     EEPROM.write(i, meas.type[i]);
        // addr += meas.type.length();

        Serial.println("committed? " + String(EEPROM.commit()));
        EEPROM.end();
        q.pop();
    }
}

// load configuration from memory to json object
void load_queue(int addr, std::queue<Measurement> &q)
{
    EEPROM.begin(MEAS_EEPROM_SIZE);
    size_t q_size;
    EEPROM.get(addr, q_size);
    addr += sizeof(q_size);
    EEPROM.commit();
    EEPROM.end();
        Serial.println("addr: " + String(addr));


    for (size_t i = 0; i < q_size; ++i)
    {
        Measurement meas;
        Serial.println("loading measurement from address " + String(addr));
        EEPROM.begin(MEAS_EEPROM_SIZE);
        meas.sensor_id = EEPROM.get(addr, meas.sensor_id);
        addr += sizeof(meas.sensor_id);
            Serial.println("addr: " + String(addr));

        // EEPROM.get(addr, meas.value);
        // addr += sizeof(meas.value);
        // EEPROM.get(addr, meas.time);
        // addr += sizeof(meas.time);

        // size_t size = meas.type.length();
        // EEPROM.get(addr, size); // write the string size
        // addr += sizeof(size);

        // // write the string itself
        // for (size_t i = addr; i < addr + size; ++i)
        //     meas.type += char(EEPROM.read(i));
        // addr += size;

        EEPROM.end();
        q.push(meas);
        print_meas(meas);
    }
}

std::queue<Measurement> queue;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.println("pushing to queue..");
    size_t size = 5;
    for (size_t i = 0; i < size; ++i)
    {
        Time time;
        time.Am = 0;
        time.hours = 11;
        time.minutes = 29;
        time.seconds = 45;
        Measurement meas;
        meas.sensor_id = i;
        meas.time = time;
        meas.type = "type" + String(i);
        meas.value = float(i);
        queue.push(meas);
        print_meas(meas);
    }

    int addr = 30;
    Serial.println("storing queue..");
    store_queue(addr, queue);

    Serial.println("queue size: " + String(queue.size()));

    Serial.println("loading queue..");
    load_queue(addr, queue);
}

void loop()
{
    delay(5000);
}