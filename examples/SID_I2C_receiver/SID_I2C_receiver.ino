// SID Wire Slave Receiver
// by Giovanni Giorgi, adapted by
// Nicholas Zambetti <http://www.zambetti.com> code examples

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 06 April 2015

// This example code is in the public domain.


#include <Wire.h>
#include <inttypes.h>
#include <SID.h>
#include <String.h>
SID mySid;

void setup()
{
  Wire.begin(23);                // join i2c bus with address #23
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  mySid.begin();
  mySid.playTestIntro();
  Serial.println("SID Slave Ready");
}

void loop()
{
  delay(100);  
  //Serial.println(".");
}

/** The I2C SID Interface is very simple: two unsigned bytes sent, register -> value
  * The I2C is a blocking interface, so we must test SID Results.
  */
void receiveEvent(int howMany)
{
  while ( Wire.available()) // loop through all but the last
  {
    uint8_t reg = Wire.read(); 
    uint8_t val = Wire.read();
    
    mySid.set_register(reg,val);
    Serial.print(reg);
    Serial.print(F(" > "));
    Serial.println(val);
  }
}
// Local variables:
// mode: c++
// mode: auto-complete
// End:
