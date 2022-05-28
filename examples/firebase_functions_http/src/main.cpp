#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

/***************************
 *  Variables Definitions For the WiFi - Change wifi ssid,password to match yours
 **************************/
#define ssid F("My_hotspot")
#define wifi_password F("mypassword")
#define ntpServer F("pool.ntp.org")
#define gmtOffset_sec 7200
#define daylightOffset_sec 7200

// you will see this url, after you deployed your functions to firebase
#define URL "https://us-central1-meshgarden-iot.cloudfunctions.net/helloWorld"
#define BURL "https://us-central1-meshgarden-iot.cloudfunctions.net/"
#define SURL "/helloWorld"

// 1
// void sendhttp()
// {
//     HTTPClient http;
//     // send http request
//     http.begin(URL);
//     int httpCode = http.GET();

//     // you don't have to handle the response, if you dont need it.
//     String payload = http.getString();
//     Serial.println(payload);

//     http.end();
// }

// 2
// void sendhttp()
// {
//     Serial.print("connecting..");

//     WiFiClientSecure client;
//     const int httpPort = 443;
//     if (!client.connect(URL, httpPort))
//     {
//         Serial.println("connection failed");
//         Serial.println(client.getWriteError());
//         return;
//     }

//     client.print(String("POST ") + URL + " HTTP/1.1\r\n" +
//                  "Host: " + URL + "\r\n" +
//                  "Connection: close\r\n\r\n");
//     unsigned long timeout = millis();

//     while (client.available() == 0)
//     {
//         if (millis() - timeout > 5000)
//         {
//             Serial.println(">>> Client Timeout !");
//             client.stop();
//             return;
//         }
//     }
//     while (client.available())
//     {
//         String line = client.readStringUntil('\r');
//         Serial.print(line);
//     }

//     Serial.println();
//     Serial.println("closing connection");
// }

// 3 V
// void sendhttp()
// {
//     if (WiFi.status() == WL_CONNECTED)
//     { // Check WiFi connection status
//         Serial.println("Still Connected !");
//         WiFiClientSecure client;
//         HTTPClient http;

//         // client.setFingerprint("81:dc:88:59:f1:fd:3b:f2:4a:27:c6:ba:39:44:3c:1c:16:4f:9c:ae");
//         client.setInsecure();
//         http.begin(client, BURL); // Specify destination for HTTP request
//         http.setURL(SURL);

//         http.addHeader("Content-Type", "application/json"); // Specify content-type header

//         int httpResponseCode = http.POST("{\"F\":\"T\"}"); // Send the actual POST request

//         if (httpResponseCode > 0)
//         {

//             Serial.println("API Called"); // Print return code
//         }
//         else
//         {

//             Serial.print("Error on sending POST: ");
//             Serial.println(httpResponseCode);
//         }

//         http.end(); // Free resources
//     }
//     else
//     {

//         Serial.println("Error in WiFi connection");
//     }
// }

// 4 
void sendhttp()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        WiFiClientSecure client;
        client.setInsecure();
        // client.setFingerprint("A5:D4:06:4C:2A:4B:46:CD:C4:A5:51:7F:4C:F6:62:92:60:90:FD:37");
        http.begin(client, URL);
        http.addHeader("Content-Type", "Content-Type: application/json");
        int httpResponseCode = http.POST("{\"F\":\"T\"}");
        if (httpResponseCode > 0)
        {
            String response = http.getString(); // Get the response to the request
            Serial.println(httpResponseCode);   // Print return code
            Serial.println(response);           // Print request answer
        }
        else
        {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
            http.end();
        }
    }
}


void setup()
{
    Serial.begin(115200);
    Serial.println();

    // Connect to Wi-Fi
    Serial.print(F("Connecting to "));
    Serial.println(ssid);
    WiFi.begin(ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println(F("WiFi connected."));

    sendhttp();
}

void loop()
{
    delay(5000);
}
