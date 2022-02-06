
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// see https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
// for a list of fonts available in the GFX lib
// or custom fonts from http://oleddisplay.squix.ch/#/home
#include<Fonts/FreeSans9pt7b.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    0 // Reset pin for display (0 or -1 if no reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // initialize serial and wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  if (!Serial) delay(3000);
  // I2C address is 0x3C, or 3D for some 128x64 modules: 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("Display setup failed");
    while (true);
  }
    // set fonts botforh display:
  display.setFont(&FreeSans9pt7b);
  Serial.println("Display is good to go");
}

void loop() {
  int sensorReading = analogRead(A0);
  
  // clear the display:
  display.clearDisplay();
  // set the text size to 1:
  display.setTextSize(1);
  // set the text color to white:
  display.setTextColor(SSD1306_WHITE);
  
  // move the cursor to 0,0:
  display.setCursor(0, 12);
  // print the seconds:
  display.print("secs:");
  display.print(millis() / 1000);
  
  // move the cursor down 20 pixels:
  display.setCursor(0, 30);
  // print a sensor reading:
  display.print("sensor:");
  display.print(sensorReading);
  // push everything out to the screen:
  display.display();
}
