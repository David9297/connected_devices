/*
  Source: https://github.com/tigoe/Wifi101_examples/blob/main/ArduinoHttpClient_Examples/HttpClientSimple/HttpClientSimple.ino
*/

/*
  Test HTTP Client
  Uses the following libraries:
  http://librarymanager/All#ArduinoHttpClient
  http://librarymanager/All#WiFi101   // use this for MKR1000
  http://librarymanager/All#WiFiNINA    // use this for MKR1010 or Nano 33 IoT
  modified 17 Jan 2021
  by Tom Igoe
*/

// include required libraries and config files
#include <WiFiNINA.h>       // use this for MKR1010 and Nano 33 IoT boards
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"

WiFiClient wifi;               // network socket to server
char server[] = "192.168.1.173";  // server name
int port = 80;

HttpClient client(wifi, server, port);      // make an HTTP client
// request timestamp in ms:
long lastRequestTime = 0;

void setup() {
  Serial.begin(9600);               // initialize serial communication
  if (!Serial) delay(3000);        // wait for serial monitor to open

  // while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);           // print the network name (SSID)
    WiFi.begin(SECRET_SSID, SECRET_PASS);         // try to connect
    delay(2000);
  }

  // When you're connected, print out the device's network status:
  Serial.print("Connected to: " );
  Serial.println(SECRET_SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int sensorValue = analogRead(A0);
    if (millis() - lastRequestTime > 5000) {
      client.get("/");  // make a GET request
  
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();
  
      Serial.println(statusCode);
      Serial.println(response);
      Serial.println(sensorValue);
      lastRequestTime = millis();
    }
}
