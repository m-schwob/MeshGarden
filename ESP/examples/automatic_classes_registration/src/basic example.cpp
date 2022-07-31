#include <Arduino.h>
#include <map>

// solution based on https://stackoverflow.com/questions/6234295/dynamically-creating-an-instance-of-a-class-from-a-string-containing-the-class-n
// TODO see if it can be done without return a pointer - should i fight it or except it?

class Device
{
public:
    virtual void sayHi() = 0;
    virtual ~Device() {}
}; // base type for all objects

struct DeviceFactory
{
    static Device *create(const String &id)
    { // creates an object from a string
        const Creators_t::const_iterator iter = static_creators().find(id);
        if (iter == static_creators().end())
        {
            Serial.println("Factory: '" + id + "' does not match any device.");
            return 0;
        }
        else
            return (*iter->second)(); // if found, execute the creator function pointer
    }

private:
    typedef Device *Creator_t();                      // function pointer to create Device
    typedef std::map<String, Creator_t *> Creators_t; // map from id to creator
    static Creators_t &static_creators()
    {
        static Creators_t s_creators;
        return s_creators;
    } // static instance of map
    template <class T = int>
    struct Register
    {
        static Device *create() { return new T(); };
        static Creator_t *init_creator(const String &id) { return static_creators()[id] = create; }
        static Creator_t *creator;
    };
};

#define REGISTER_DEVICE(T, STR) template <> \
                                DeviceFactory::Creator_t *DeviceFactory::Register<T>::creator = DeviceFactory::Register<T>::init_creator(STR)

class SensorA : public Device
{
public:
    SensorA() { Serial.println("SensorA constructor"); }
    void sayHi()
    {
        Serial.println("SensorA say hi!");
    }
};

REGISTER_DEVICE(SensorA, "A");

class SensorB : public SensorA // test two steps inheriting
{
public:
    SensorB() { Serial.println("SensorB constructor"); }
    void sayHi()
    {
        Serial.println("SensorB say hi!");
    }
};

REGISTER_DEVICE(SensorB, "B");

class SensorC : public Device
{
public:
    SensorC() { Serial.println("SensorC constructor"); }
    void sayHi()
    {
        Serial.println("SensorC say hi!");
    }
};

REGISTER_DEVICE(SensorC, "C");

class SensorD : public Device
{
public:
    SensorD() { Serial.println("SensorD constructor"); }
    void sayHi()
    {
        Serial.println("SensorD say hi!");
    }
};

REGISTER_DEVICE(SensorD, "D");

void heap_status()
{
    Serial.print(F("FreeHeap "));
    Serial.println(ESP.getFreeHeap());
}

void _setup()//TODO function name changed temporary to prevent multiple definition error
{
    Serial.begin(115200);
    Serial.println();

    heap_status();
    Device *a = DeviceFactory::create("A");
    Device *b = DeviceFactory::create("B");
    Device *c = DeviceFactory::create("C");
    Device *d = DeviceFactory::create("D");
    Device *e = DeviceFactory::create("E");

    heap_status();
    a->sayHi();
    b->sayHi();
    c->sayHi();
    d->sayHi();
    Serial.println(String(e == NULL));

    heap_status();
    delete a;
    delete b;
    delete c;
    delete d;

    heap_status();
}

void _loop() //TODO function name changed temporary to prevent multiple definition error
{
    delay(5000);
}