#include <SID.h>

/************************************************************************

        Arduino controls an
	Atmega8 MOS6581 SID Emulator
        by softare serial line
        
	( SID = Sound Interface Device )

         random sound pitch example fro demostration of Stereo Effect
 Sid voidce 2 is controlled by pin 10
 The sketch push sound on channel 1 (pin9) and after 2 seconds on channel 2 (pin10)
 
        


****************************************************************************

	date	authors					version		comment
	======	======================	=======		==============================
	2017 	Giovanni Giorgi         V1.0		First version based on
                                                        Christoph Haberer implementation

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
#define OFF 0
#define SETTRIANGLE_1	4,0x11,5,0xBB,6,0xAA,
#define C4_1	1,0x11,0,0x25,
#define CONTROLREG 4 // SID control register address

// waveforms
#define SETNOISE_1 	4,0x81,5,0xBB,6,0xAD, // SID register setup to create noise

SID mySid;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
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
void setwaveform_triangle(uint8_t channel)
{
  uint8_t dataset[]={ SETTRIANGLE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
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

uint8_t zufall()
{
	static uint16_t startwert=0x0AA;

	uint16_t temp;
	uint8_t n;
		
	for(n=1;n<8;n++)
	{
		temp = startwert;
		startwert=startwert << 1;
	
		temp ^= startwert;
		if ( ( temp & 0x4000 ) == 0x4000 ) 
		{ 
			startwert |= 1;
		}
	}
	
	return (startwert);
}
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14

/************************************************************************
	
    main loop

    create space ship noise
    
************************************************************************/        
const int SECONDS_PERIOD=2;
void loop()
{
  uint16_t n;
  uint8_t flag,k;
  uint16_t soundindex=0;

  

  setwaveform_triangle(CHANNEL1);
  setwaveform_triangle(CHANNEL2);

  boolean switchChannel=false;
  int c;
  digitalWrite(LED_BUILTIN, HIGH);  
  set_frequency(0,CHANNEL1);
  set_frequency(0,CHANNEL2);
  c=SECONDS_PERIOD*10;
  while(--c>0)
  {
    n=zufall()*8;    
    set_frequency(n*17,CHANNEL2); // Voce 2 Channel (pin10)
    n=zufall()*2;    
    delay(100);
  } 
  set_frequency(0,CHANNEL1);
  set_frequency(0,CHANNEL2);
  digitalWrite(LED_BUILTIN, LOW);  
  // then invert 
  c=SECONDS_PERIOD*10;
  while(--c>0)
  {
    n=zufall()*8;    
    set_frequency(n*17,CHANNEL1); // change noise generator frequency    
    n=zufall()*2;    
    delay(100);
  }  

}

