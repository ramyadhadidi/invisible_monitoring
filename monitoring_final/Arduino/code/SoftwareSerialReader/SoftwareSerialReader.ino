/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */
#include <SoftwareSerial.h>
char c;
SoftwareSerial mySerial(8, 9); // RX, TX

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  mySerial.begin(9600);
  
  // make the pushbutton's pin an input:
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  if (mySerial.available()>0){
    c=mySerial.read();
    Serial.print(c);
  }
}



