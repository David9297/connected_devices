/* sources:
   1. https://github.com/tigoe/hid-examples/blob/main/KeyboardFourPushbuttons/KeyboardFourPushbuttons.ino
   2. https://github.com/tigoe/hid-examples/blob/main/MouseJoystick/MouseJoystick.ino
*/

#include <Keyboard.h>
#include <Mouse.h>

int redLEDPin = 7;
int greenLEDPin = 8;
int blueLEDPin = 9;

int redLEDState = LOW;
int greenLEDState = LOW;
int blueLEDState = LOW;

// the enable button should be on a hardware interrupt
// pin. Check your board's reference to find them:
const int HIDEnableButton = 2;
int lastHIDEnableButtonState = HIGH;
int currentHIDEnableButtonState;

// whether or not HID is enabled:
volatile bool HIDEnabled = false;

// for joystick pushbutton
int lastButton = HIGH;

// debounce delay for pushbutton:
const int debounceDelay = 4;
int buttonCount = 4;
//  pushbuttons are on these pins:
int buttonPin[] = {3, 4, 5, 6};
// previous states of the buttons:
int lastButtonState[] = {HIGH, HIGH, HIGH, HIGH};
// characters to print when the buttons are pushed:
char buttonChar[] = {'w', 'a', 's', 'd'};
int ledState = LOW;

void setup() {
  // initialize serial and Keyboard:
  Serial.begin(9600);

  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

  // set up the pinMode and interrupt service routine:
  pinMode(HIDEnableButton, INPUT_PULLUP);
  attachInterrupt(HIDEnableButton, HIDChange, FALLING);
  // Keyboard.begin();
  currentHIDEnableButtonState = digitalRead(HIDEnableButton);

  // configure analog pins for Playstation joystick:
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  // joystick pushbutton:
  pinMode(A2, INPUT_PULLUP);
  // turn on A0 for +v connection of joystick:
  digitalWrite(A0, HIGH);
  // turn off A1 for gnd connection of joystick:
  digitalWrite(A1, LOW);

  // set button input modes:
  for (int b = 0; b < buttonCount; b++) {
    pinMode(buttonPin[b], INPUT_PULLUP);
  }
}

void loop() {
  HIDChange();
  if (HIDEnabled) {
    // read the inputs on analog pin 0 and 1:
    int x = analogRead(A0);
    delay(1);
    int y = analogRead(A1);
    // read the pushbutton:
    int button = !digitalRead(A2);

    // map x and y readings to a 3-point range
    // and subtract 1 to get -3 to 3, with
    // 0 at rest. You may have to modify the numbers
    // to make it work:
    x = map(x, 0, 1023, -4, 3) + 1;
    y = map(y, 0, 1023, -4, 3) + 1;

    // move the mouse:
    Mouse.move(x * 2, y * 2, 0);

    // if the button changes, press or release the mouse,
    // as appropriate.  LOW to HIGH = press, HIGH to LOW = release:
    if (button != lastButton) {
      if (button == HIGH) {
        Mouse.press(MOUSE_LEFT);
      }
      else {
        Mouse.release(MOUSE_LEFT);
      }
    }
    // print the results
    Serial.print(x);
    Serial.print("\t");
    Serial.print(y);
    Serial.print("\t");
    Serial.println(button);
    // store the button state for comparison next time:
    lastButton = button;

    // iterate over the button list:
    for (int b = 0; b < buttonCount; b++) {
      // read this button:
      int buttonState = digitalRead(buttonPin[b]);
      // if it has changed:
      if (buttonState != lastButtonState[b]) {
        // wait until button state stabilizes:
        delay(debounceDelay);
        // if it's low:
        if (buttonState == HIGH) {
          // send keystroke:
          Keyboard.press(buttonChar[b]);
          delay(100);
          Keyboard.release(buttonChar[b]);
          Serial.print(buttonChar[b]);
        }
        // save current state for next time:
        lastButtonState[b] = buttonState;
      }
    }
  }
}

// interrupt handler for the HID Enable button:
void HIDChange() {
  // the current state of LED
  lastHIDEnableButtonState = currentHIDEnableButtonState;      // save the last state
  currentHIDEnableButtonState = digitalRead(HIDEnableButton); // read new state

  if (currentHIDEnableButtonState != lastHIDEnableButtonState) {
    if (currentHIDEnableButtonState == HIGH) {
      Serial.println("The button is pressed");
      ledState = !ledState;
      Serial.print("ledstate: ");
      Serial.println(ledState);
      HIDEnabled = !HIDEnabled;
  
    }
    lastHIDEnableButtonState = currentHIDEnableButtonState; 
  }
  if (ledState == true) {
    RGB_color(0, 255, 0);
  } else {
    RGB_color(255, 0, 0);
  }

  // debounce:
  delay(debounceDelay);

  if (HIDEnabled) {
    Serial.print("HID is enabled");
    Mouse.begin();
    Keyboard.begin();

  } else {
    Serial.print("HID is disabled");
    Mouse.end();
    Keyboard.end();
  }
}

void RGB_color(int redLEDPinValue, int greenLEDPinValue, int blueLEDPinValue) {
  analogWrite(redLEDPin, redLEDPinValue);
  analogWrite(greenLEDPin, greenLEDPinValue);
  analogWrite(blueLEDPin, blueLEDPinValue);
}
