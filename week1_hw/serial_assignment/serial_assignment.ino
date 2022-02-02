//adding JSON library for Arduino
#include <Arduino_JSON.h>

// variables to hold previous knob states
int lastKnobState1 = 0;
int lastKnobState2 = 0;
int lastKnobState3 = 1;
// threshold of change for the knobs (range 0-100):
int threshold = 3;

// if the inputs have changed:
bool inputsChanged = false;
// a JSON object to hold the data to send:
JSONVar outgoing;

void setup() {
  // initialize serial and set input pin modes:
  Serial.begin(9600);
  
  // initialize value in JSON object:
  outgoing["knob1"] = 0;
  outgoing["knob2"] = 0;
  outgoing["knob3"] = 1;
}

void loop() {
  // read inputs from knobs, map if needed:
  int knobState1 = analogRead(A0);
  //knobState1 = map(knobState1, 0, 1023, 0, 10);
  
  int knobState2 = analogRead(A1);
 // knobState2 = map(knobState2, 0, 1023, 0, 20);

  int knobState3 = analogRead(A2);
  //knobState3 = map(knobState3, 0, 1023, 1, 21);

   // if knob 1 has changed:
  if (abs(knobState1 - lastKnobState1) > threshold) {
     knobState1 = map(knobState1, 0, 1023, 0, 10);
    outgoing["knob1"] = knobState1;
    // save knob 1 state for comparison next time:
    lastKnobState1 = knobState1;
    // set change flag so serial will send:
    inputsChanged = true;
  }

   // if knob 2 has changed:
  if (abs(knobState2 - lastKnobState2) > threshold) {
    knobState2 = map(knobState2, 0, 1023, 0, 20);
    outgoing["knob2"] = knobState2;
    // save knob 2 state for comparison next time:
    lastKnobState2 = knobState2;
    // set change flag so serial will send:
    inputsChanged = true;
  }

     // if knob 3 has changed:
  if (abs(knobState3 - lastKnobState3) > threshold) {
    knobState3 = map(knobState3, 0, 1023, 1, 21);
    outgoing["knob3"] = knobState3;
    // save knob 3 state for comparison next time:
    lastKnobState3 = knobState3;
    // set change flag so serial will send:
    inputsChanged = true;
  }
  if (inputsChanged) {
    Serial.println(outgoing);
    inputsChanged = false;
  }
}
