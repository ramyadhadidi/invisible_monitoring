/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <XBee.h>
#include <SoftwareSerial.h>
/*This is the node that the arduino is going to send on the MSB of 
the payload, to identify it on the receiver, it  has to be unique 
in the configuration
Max Value 63
*/

#define NODE_NUMBER 3
//Coordinator numbers
#define SH 0x0013a200
#define SL 0x40605FF8 
//Pin number to connect the sensor
#define PIN_NUMBER A0
///Delay between messages in milliseconds
#define DELAY 500
///Delay between readings in milliseconds
#define READS_DELAY 10
/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(pin5) and checks the status response for success
*/

// create the XBee object
XBee xbee = XBee();

//Each load is 8-bits
uint8_t payload[] = { 0, 0 };

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(SH, SL);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

// Define NewSoftSerial TX/RX pins
// Connect Arduino pin 8 to TX of usb-serial device
uint8_t ssRX = 8;
// Connect Arduino pin 9 to RX of usb-serial device
uint8_t ssTX = 9;
// Remember to connect all devices to a common Ground: XBee, Arduino and USB-Serial device
SoftwareSerial nss(ssRX, ssTX);

int pin5 = 0;
/*Temporal*/
int counter = 0;
int value = 0;
int temp=0;
/*End of Temporal*/
int statusLed = 13;
int errorLed = 13;

void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  nss.begin(9600);
  Serial.begin(9600);
  xbee.setSerial(Serial);
  pinMode(12, OUTPUT);  
  digitalWrite(12, HIGH);  
  counter = 0;
}

void loop() {   
  // break down 10-bit reading into two bytes and place in payload
  pin5 = analogRead(PIN_NUMBER);
  if((temp-pin5) > 100){
    value=256;
  }
  else if((pin5-temp) > 100){
    value=256;
  }

  if(counter > DELAY){
    payload[0] = 0x00;
    //First two bits are the MSF of pin5, the other 6 are the node number
    payload[0] = payload[0]|(value >> 8 & 0x03)|(NODE_NUMBER<<2 & 0xFC);
    payload[1] = value & 0xff;///Less significant bits

    xbee.send(zbTx);

    // flash TX indicator
    flashLed(statusLed, 1, 100);
    nss.println("Test");
    // after sending a tx request, we expect a status response
    // wait up to half second for the status response
    if (xbee.readPacket(500)) {
      // got a response!

      // should be a znet tx status            	
      if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
        xbee.getResponse().getZBTxStatusResponse(txStatus);

        // get the delivery status, the fifth byte
        if (txStatus.getDeliveryStatus() == SUCCESS) {
          // success.  time to celebrate
          nss.println("Success");
          flashLed(statusLed, 5, 50);
        } else {
          // the remote XBee did not receive our packet. is it powered on?
          flashLed(errorLed, 3, 500);
          nss.println("Packet not received");        
        }
      }
    } else if (xbee.getResponse().isError()) {
      //nss.print("Error reading packet.  Error code: ");  
      nss.println(xbee.getResponse().getErrorCode());
    } else {
      // local XBee did not provide a timely TX Status Response -- should not happen
      flashLed(errorLed, 2, 50);
      nss.println("Nobody there");
    }
    counter = 0;
    value=0;
  }
  counter=counter+READS_DELAY;
  temp = pin5;
  delay(READS_DELAY);
}

