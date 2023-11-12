// SID Wire Slave Receiver
// by Giovanni Giorgi, adapted by
// Nicholas Zambetti <http://www.zambetti.com> code examples

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Also, this version support "bulk" sends but remember wire library implementation uses a 32 byte buffer
// so you cannot send more then 16 register commands (i.e. 32 bytes)

// Last revision April 2018
// Created 06 April 2015
// This example code is in the public domain.


#include <Wire.h>
#include <inttypes.h>
#include <SID.h>
#include <String.h>
SID mySid;
bool ledOn=false;

void setup()
{
  Wire.begin(23);                // join i2c bus with address #23
  Wire.onReceive(receiveEvent); // register event  
  mySid.begin();
  pinMode(LED_BUILTIN, OUTPUT); // TRUE ON ONBULK SEND
  mySid.loadPiano(VOICE1_Right);
  mySid.loadPiano(VOICE2_Left);  
  // mySid.playTestIntro();  
}

void loop()
{  
  if(ledOn) {
      digitalWrite(LED_BUILTIN, HIGH); 
  }else{
      digitalWrite(LED_BUILTIN, LOW); 
  }   
}

/** The I2C SID Interface is very simple: two unsigned bytes sent, register -> value
  * The I2C is a blocking interface.
  * To speed up things the caller can send more then 2 bytes: we turn on the led 13 to signal this "bulk" mode
  * 
  */
void receiveEvent(int howMany)
{  
  if( Wire.available() >2 ){
    ledOn=true;
  }else{
    ledOn=false;
  }
  while ( Wire.available()) // loop through all 
  {
    uint8_t reg = Wire.read(); 
    uint8_t val = Wire.read();    
    mySid.set_register(reg,val);        
  }
}
// Local variables:
// mode: c++
// mode: auto-complete
// End:
