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

/*
This example is for Series 2 XBee
Receives a ZB RX packet and prints the packet to softserial
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

// Define NewSoftSerial TX/RX pins
// Connect Arduino pin 8 to TX of usb-serial device
uint8_t ssRX = 8;
// Connect Arduino pin 9 to RX of usb-serial device
uint8_t ssTX = 9;
// Remember to connect all devices to a common Ground: XBee, Arduino and USB-Serial device
SoftwareSerial nss(ssRX, ssTX);

//Data temporal varialbe
int data = 0;

void setup() {  
  // start serial
  Serial.begin(9600);
  xbee.setSerial(Serial);
  nss.begin(9600);
  
  nss.println("Starting up!");
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
           
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
                
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            // the sender got an ACK
        } else {
          nss.println("packet not acknowledged");
        }
         //Always the first package is the ID of the sender node
        //The package is always <-6bits:NodeNumber-><-10-bits:data->
         nss.print("Hello sender: ");
         nss.println((rx.getData())[0]>>2&0xFF);
         data = 0;
         data = data|( (((rx.getData())[0]&0x03)<<8) + ((rx.getData())[1]&0xFF));
         nss.print("Data:");
         nss.println(data);
       }
    } else if (xbee.getResponse().isError()) {
      nss.print("oh no!!! error code:");
      nss.println(xbee.getResponse().getErrorCode());
    }
}
