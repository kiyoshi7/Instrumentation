// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//
#include <Arduino.h>
#include <SPI.h>
#include "Interpreter.h"
Interpreter SIn;

// Define Functions below here or use other .ino or cpp files
//
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// The setup() function runs once each time the micro-controller starts
void setup()
{  
  SIn.Debug(true);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  SIn.Setup(SPI);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
}

// Add the main program code into the continuous loop() function
void loop()
{
  if (stringComplete) {
    SIn.Interpret(inputString);
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      inputString.remove( inputString.length()-2,2 );// remove the newline
      if (inputString.length()>0){stringComplete = true;} // make sure data is larger than 4 and effectivly delete data
    }
  }
}
