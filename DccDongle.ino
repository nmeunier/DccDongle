/*
   Copyright 2014 Nicolas Meunier

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/**
  DCC signal generator Firmware for Arduino.
  Connect Arduino board to USB port and send commands via serial port.
  This code need the CmdrArduino library from Railstars for generate DCC signal. See :  https://github.com/Railstars/CmdrArduino
  The DCC signal output is on Pin 9 (Pin 11 for Arduino Mega), and is suitable for connection to an LMD18200-based booster.
**/

#include <DCCHardware.h>
#include <DCCPacket.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>


DCCPacketScheduler dps;
byte count = 0;
String inputString = "";         // a string to hold incoming data 
boolean stringComplete = false;  // whether the string is complete
String cmd = "";
String args = "";
unsigned int adr = 0;
unsigned int val = 0;


void setup() {
  Serial.begin(115200);
  dps.setup();
}

void loop() {

  if (stringComplete) {
  
	  //Parse input string
      cmd = inputString.substring(0,(inputString.indexOf(" ")));
      inputString = inputString.substring((inputString.indexOf(" ") +1));
	  
	  //Message debug info 
      Serial.print("cmd =");
      Serial.print(cmd);
      Serial.print(" ");
	  
      adr = inputString.substring(0,(inputString.indexOf(" "))).toInt();
      inputString = inputString.substring((inputString.indexOf(" ") + 1));

      val = inputString.toInt();
	  
	  //Message debug info 
      Serial.print(adr);
      Serial.print(" ");
      Serial.print(val);
      Serial.print("\n");
	  
      if (cmd == "setSpeed128")
      {
          Serial.println("setSpeed128");
          dps.setSpeed128(adr,DCC_LONG_ADDRESS,val);
      }
      else if (cmd == "setFunctions0to4")
      {
          Serial.println("setFunctions0to4");
          dps.setFunctions(adr,DCC_LONG_ADDRESS,val);
      }
      else if (cmd == "setAccessory")
      {
        
          if(val == 1){
            
              dps.setBasicAccessory((adr >> 2) + 1, ((adr - 1) & 0x03) << 1);
              
          }else{ 

              dps.unsetBasicAccessory((adr >> 2) + 1, ((adr - 1) & 0x03) << 1);
          
          }
      }

      // clear the string:
      inputString = "";
      cmd = "";
      args = "";
      stringComplete = false;
  }
 
  dps.update();  
  ++count;
}

void serialEvent() {
  while (Serial.available()) {
    // get new data in serial
    char inChar = (char)Serial.read(); 

	//Test if new line
    if ((inChar == '\r') || (inChar == '\n')) {
      stringComplete = true;
    }else{
      inputString += inChar;
    }
  }
}

