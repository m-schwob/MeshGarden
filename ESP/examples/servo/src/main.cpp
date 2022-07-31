#include <Servo.h>

Servo myservo; // create servo object to control a servo

int potpin = 0; // analog pin used to connect the potentiometer
int val;        // variable to read the value from the analog pin

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    myservo.attach(D4); // attaches the servo on pin 9 to the servo object
}

void loop()
{
    // val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
    // val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    Serial.println("1");
    digitalWrite(LED_BUILTIN, LOW);
    myservo.write(180); // sets the servo position according to the scaled value
    delay(5000);

    // waits for the servo to get there
    Serial.println("2");

    digitalWrite(LED_BUILTIN, HIGH);
    myservo.write(0); // sets the servo position according to the scaled value
    delay(10000);     // waits for the servo to get there
}