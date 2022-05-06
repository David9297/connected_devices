/* sources:
   1. https://github.com/tigoe/hid-examples/blob/main/KeyboardFourPushbuttons/KeyboardFourPushbuttons.ino
   2. https://github.com/tigoe/hid-examples/blob/main/MouseJoystick/MouseJoystick.ino
   3. https://github.com/tigoe/Wifi101_examples/blob/main/ArduinoHttpClient_Examples/HttpClientSimple/HttpClientSimple.ino
   4. https://github.com/dombarrett/Fast-Fun-SP2022/blob/main/WiFi_OSC_Example.ino
*/

#include <SPI.h>
#include <ArduinoOSCWiFi.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

// WiFi stuff
const char* ssid = SECRET_SSID; //YOUR WifiSSD name
const char* pwd = SECRET_PASS; //YOUR PASSWORD WATCH OUT
const IPAddress ip(192, 168, 1, 201); //The local ip of the Arduino is whatever you say it is
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

const char* host = "192.168.1.45";
const int publishPort = 54445;  //the port we will use to receive on the computer

int joystickX = A0;
int joystickY = A1;
int joystickButtonPin = A2;

int redLEDPin = 3;
int greenLEDPin = 4;
int blueLEDPin = 5;

int redLEDState = LOW;
int greenLEDState = LOW;
int blueLEDState = LOW;

// the enable button should be on a hardware interrupt
// pin. Check your board's reference to find them:
const int OSCEnableButton = 2;
int lastOSCEnableButtonState = HIGH;
int currentOSCEnableButtonState;

// whether or not OSC is enabled:
volatile bool OSCEnabled = false;

// debounce delay for pushbutton:
const int debounceDelay = 4;
int buttonCount = 4;
//  pushbuttons are on these pins:
int buttonPin[] = {6, 7, 8, 9};
// previous states of the buttons:
int lastButtonState[] = {HIGH, HIGH, HIGH, HIGH};
int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;
int joystickXState = 0;
int joystickYState = 0;
int joystickPressState = 0;

// characters to print when the buttons are pushed:
char buttonChar[] = {'w', 'a', 's', 'd'};
int ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

  // set up the pinMode and interrupt service routine:
  pinMode(OSCEnableButton, INPUT_PULLUP);
  attachInterrupt(OSCEnableButton, OSCChange, FALLING);
  currentOSCEnableButtonState = digitalRead(OSCEnableButton);

  // configure analog pins for Playstation joystick:
  pinMode(joystickX, OUTPUT);
  pinMode(joystickY, OUTPUT);
  // joystick pushbutton:
  pinMode(joystickButtonPin, INPUT_PULLUP);
  // turn on A0 for +v connection of joystick:
  digitalWrite(A0, HIGH);
  // turn off A1 for gnd connection of joystick:
  digitalWrite(A1, LOW);

  // set button input modes:
  for (int b = 0; b < buttonCount; b++) {
    pinMode(buttonPin[b], INPUT_PULLUP);
  }
  connectToNetwork();

  OscWiFi.publish(host, publishPort, "/publish/func", currentOSCEnableButtonState, button1State, button2State, button3State, button4State, joystickXState, joystickYState, joystickPressState)
   ->setFrameRate(60.f);
}

void loop() {
  OSCChange();
  checkControls();
}

void connectToNetwork() {
  // try to connect to the network:
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to: " + String(SECRET_SSID));
    //Connect to WPA / WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    WiFi.config(ip, gateway, subnet);
    delay(2000);
  }
  Serial.println("Connected to: " + String(SECRET_SSID));
  
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void checkControls() {
  if(digitalRead(buttonPin[0]) == LOW) {
    button1State = 0;
  } else {
    button1State = 1;
  }

  if(digitalRead(buttonPin[1]) == LOW) {
    button2State = 0;
  } else {
    button2State = 1;
  }

  if(digitalRead(buttonPin[2]) == LOW) {
    button3State = 0;
  } else {
    button3State = 1;
  }

 if(digitalRead(buttonPin[3]) == LOW) {
    button4State = 0;
  } else {
    button4State = 1;
  }

  if(digitalRead(joystickButtonPin) == LOW) {
    joystickPressState = 1;
  } else {
    joystickPressState = 0;
  }

  //read the inputs on analog pin 0 and 1:
  int x = analogRead(joystickX);
  delay(1);
  int y = analogRead(joystickY);

  // map x and y readings to a 3-point range
  x = map(x, 0, 1023, -4, 3) + 1;
  y = map(y, 0, 1023, -4, 3) + 1;

  joystickXState = x;
  joystickYState = y;

  Serial.println(joystickXState);
  Serial.println(joystickYState);
}

void OSCChange() {
  //the current state of LED
  lastOSCEnableButtonState = currentOSCEnableButtonState;      // save the last state
  currentOSCEnableButtonState = digitalRead(OSCEnableButton); // read new state

  if (currentOSCEnableButtonState != lastOSCEnableButtonState) {
    if (currentOSCEnableButtonState == HIGH) {
      Serial.println("The button is pressed");
      ledState = !ledState;
      Serial.print("ledstate: ");
      Serial.println(ledState);
      OSCEnabled = !OSCEnabled;
    }
    lastOSCEnableButtonState = currentOSCEnableButtonState;
  }
  if (ledState == true) {
    RGB_color(0, 255, 0);
  } else {
    RGB_color(255, 0, 0);
  }

  // debounce:
  delay(debounceDelay);

  if (OSCEnabled) {
    OscWiFi.update();
  }
}

void RGB_color(int redLEDPinValue, int greenLEDPinValue, int blueLEDPinValue) {
  analogWrite(redLEDPin, redLEDPinValue);
  analogWrite(greenLEDPin, greenLEDPinValue);
  analogWrite(blueLEDPin, blueLEDPinValue);
}
