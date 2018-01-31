#include <WebUSB.h>

WebUSB WebUSBSerial(1, "webusb.github.io/arduino/demos");

#define Serial WebUSBSerial

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the indicator LED pin


int color[3];
int index;

int ledState = HIGH;        // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  while (!Serial) {
    ;
  }
  Serial.begin(9600);
  Serial.write("Sketch begins.\r\n");
  Serial.flush();
  index = 0;

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only send a message if the button state has changed
      if (buttonState == HIGH) {
        ledState = !ledState;

        if (Serial && Serial.available()) {
          Serial.print("Indicator LED state: ");
          Serial.print(ledState);
          Serial.print(".\r\n");
          Serial.flush();
        }
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;


  // webUSB serial stuff
  if (Serial && Serial.available()) {
    color[index++] = Serial.read();
    if (index == 3) {
      analogWrite(redPin, color[0]);
      analogWrite(greenPin, color[1]);
      analogWrite(bluePin, color[2]);
      Serial.print("Set LED to ");
      Serial.print(color[0]);
      Serial.print(", ");
      Serial.print(color[1]);
      Serial.print(", ");
      Serial.print(color[2]);
      Serial.print(".\r\n");
      Serial.flush();
      index = 0;
    }
  }
}
