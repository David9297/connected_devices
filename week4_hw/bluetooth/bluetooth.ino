/*Source code:
 * https://github.com/tigoe/html-for-conndev/blob/main/bluetooth-p5ble/ReadOneCharacteristic/SensorCharacteristic/SensorCharacteristic.ino
 */

#include <ArduinoBLE.h>

const int potPin1 = LED_BUILTIN; // set ledPin to on-board LED
const int potPin2 = LED_BUILTIN; // set ledPin to on-board LED
const int potPin3 = LED_BUILTIN; // set ledPin to on-board LED

// create service:
BLEService sensorService("19b10010-e8f2-537e-4f6c-d104768a1214");
// create sensor characteristic and allow remote device to get notifications:
BLEStringCharacteristic sensorCharacteristic("19b10012-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify, 15);

int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 1;
int sensorPin1 = A0;  
int sensorPin2 = A1;
int sensorPin3 = A2;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(potPin1, OUTPUT); // use the LED as an output
  pinMode(potPin2, OUTPUT); // use the LED as an output
  pinMode(potPin3, OUTPUT); // use the LED as an output

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (true);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("SensorPeripheral");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(sensorService);

  // add the characteristic to the service
  sensorService.addCharacteristic(sensorCharacteristic);

  // add the service
  BLE.addService(sensorService);

  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();

  String previousMessage = "";

  if (millis() % 500 < 2) {
    String message = "";
    // read the value from the sensor:
    sensorValue1 = analogRead(sensorPin1);
    sensorValue1 = map(sensorValue1, 0, 1023, 0, 10);
    Serial.println(sensorValue1);
    message = message + String(sensorValue1) + ":";
    
    // read the value from the sensor:
    sensorValue2 = analogRead(sensorPin2);
    sensorValue2 = map(sensorValue2, 0, 1023, 0, 20);
    Serial.println(sensorValue2);
    message = message + String(sensorValue2) + ":";

    // read the value from the sensor:
    sensorValue3 = analogRead(sensorPin3);
    sensorValue3 = map(sensorValue3, 0, 1023, 1, 21);
    Serial.println(sensorValue3);
    message = message + String(sensorValue3);

    if (previousMessage != message) {
      sensorCharacteristic.writeValue(message);
    }
    previousMessage = message;
  }
}
