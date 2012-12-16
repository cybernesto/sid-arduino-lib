#include <SID.h>

/************************************************************************

        Arduino controls an
	Atmega8 MOS6581 SID Emulator
        
	( SID = Sound Interface Device )

        This example sets a noise sound in the SID and changes the frequency 
        from low to high. 
        
        This creates a kind of space sound as used in the 1980 games.

****************************************************************************

	date	authors					version		comment
	======	======================	=======		==============================
	2007 	(ch) Christoph Haberer	V1.0		First implemetation
        2012    Mario Patino            V1.1            Ported to SID Library
	Versions:

	V1.0
	- noise with variable frequency

        V1.1
        - The SID chip is implemented as a library thus making the serial
        communication unnecessary

***************************************************************************/
/***************************************************************************
 *   
 *   (c) 2007 Christoph Haberer, christoph(at)roboterclub-freiburg.de
 *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation version 2 of the License,                *
 *   If you extend the program please maintain the list of authors.        *
 *   If you want to use this software for commercial purposes and you      *
 *   don't want to make it open source, please contact the authors for     *
 *   licensing.                                                            *
 ***************************************************************************/

#define C4_1	1,0x11,0,0x25, // C4 tone pitch
#define OFF 0
#define CONTROLREG 4 // SID control register address
#define GATE (1<<0)

// waveforms
#define SETNOISE_1 	4,0x81,5,0xBB,6,0xAD, // SID register setup to create noise



SID mySid;

void setup() {
  digitalWrite(13, HIGH); //turn on debugging LED
  mySid.begin();
}


/************************************************************************
	
    void setwaveform_noise(uint8_t channel)
    
    Set sid registers to create a noise waveform.
    The SID has 3 channels, which should be accesed according to this
    definition:
    
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    
    For noise sounds it makes sense to use only one channel, because 
    using multiple channels gives almost no audible difference.
    
************************************************************************/
void setwaveform_noise(uint8_t channel)
{
  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0;  
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2; 
  }
}

  
  /************************************************************************
	
    void set_frequency(uint16_t pitch,uint8_t channel)
    
    Set sid frequncy registers.
    
    The pitch has to be:
    
    pitch=16.77*frequency
    
    The SID has 3 channels, which should be accesed according to this
    definition:
    
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    
************************************************************************/
// pitch=16.77*frequency
void set_frequency(uint16_t pitch,uint8_t channel)
{
    mySid.set_register(channel, pitch&0xFF); // low register adress
    mySid.set_register(channel+1, pitch>>8); // high register adress
}

#define CHANNEL1  0

/************************************************************************
	
    main loop

    create space ship noise
    
************************************************************************/        
void loop()
{
  uint16_t n;
  uint8_t flag,k;
  uint16_t soundindex=0;
  
  setwaveform_noise(CHANNEL1);

  for(k=0;k<10;k++) // ten times space ship noise
  {
    for(n=100;n<1000;n+=50)
    {
       set_frequency(n*17,CHANNEL1); // change noise generator frequency
       delay(50);// every 50ms
    }
  }
  mySid.set_register(CONTROLREG+CHANNEL1, OFF);   // be quite SID!

  while(1); // do nothing and loop forever
}
