/*
 SID.cpp - Atmega8 MOS6581 SID Emulator Version 1.9
 Copyright (c) 2007 Christoph Haberer, christoph(at)roboterclub-freiburg.de
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
/************************************************************************

	Atmega8 MOS6581 SID Emulator

	SID = Sound Interface Device

	This program tries to emulate the sound chip SID of the famous 
	historical C64 Commodore computer.
	The SID emulator includes all registers of the original SID, but 
	some functions may not be implemented yet.
	If you want	to program the SID registers to generate your own sound,
	please refer to the MOS6581 datasheet. The emulator tries to be as
	compatible as possible.

	In the main program there is an interrupt routine which sets the 
	ouptut values for the PWM-Output at 62.5kHz. Therefore the high 
	frequency noise	of the PWM should not be audible to normal people.
	The output is calculated with a 16kHz sample frequency to save 
	processing cycles.
	
	The envelope generators are updated every 1ms. 

	The amplitude value is output as an 8Bit PWM value. 
	The PWM-Output may be directly connected to an audio amplifier.
	
************************************************************************

	Hardware
	
	processor:	ATMEGA8, ATMEGA168 ( see #define )
	clock:		16MHz Crystal

	PIN17	PB1/OC2(A)		8Bit PWM sound output
	PIN19	PB0				test LED ( till version 1.7 Pin 14 was used )

****************************************************************************

	date	authors					version		comment
	======	======================	=======		==============================
	Apr.07	(ch) Christoph Haberer	V1.0		First implemetation
	Apr.22	(ch) Christoph Haberer	V1.2		some improvements
	Apr.25	(ch) Christoph Haberer	V1.4		ATMEGA168 added
	Jul.20	(ch) Christoph Haberer	V1.6		serial comunication
	Jul.28	(ch) Christoph Haberer	V1.7		some bugs removed
	Sep.28	(ch) Christoph Haberer	V1.8		additional gates
	Nov.4 2012   Mario Patino       V1.9        arduino library conversion
	
	Versions:

	V1.0
	- 3 waveform generators
	- 3 envelope generators with attack,decay, sustain, release
	- waverforms: triangle, rectangle with variable pulse width,
	  sawtooth, noise with variable frequency
	- ring modulation 
	- not implemented yet: prgrammable digital filter

	V1.2
	- some errors in the envelope generators removed
	- sustain levels initialized to maximum =0x0F
	- direct output/filter select bits implemented
	- short delay comand "0xF3,time[ms]," included in scheduler 
	
	V1.4 
	- ATMEGA168 added

	V1.6
	- serial comunication added, 9600 Baud, 
	first Byte: adress, second byte register value

	V1.8
	- LED Pin changed to from Pin 14 to Pin19 to be Arduino compatibel
	The Arduino has a dedicated LED resistor at PIN19
	3 additional LEDs to show the state of the gates
	
	V1.9
	- Encapsulated methods in a library to be able to use the SID directly 
	in Arduino. 
	- Removed unnecesary code
	- Optimized timer usage to avoid the need of additional wiring
	- Changed PWM output to Timer 2 OC2(A) to leave the 16bit timer 1 as 
	a frequency generator for the 16kHz and 1kHz interrupts
	- Changed the noise generator to a Galois LFSR
	- Replaced the DDS wavetables with direct calculation to save RAM

***************************************************************************/

#include <avr/interrupt.h>
#include <Arduino.h>

#include "SID.h"

// attack, decay, release envelope timings
const static uint16_t AttackRate[16]={2,4,16,24,38,58,68,80,100,250,500,800,1000,3000,5000,8000};
const static uint16_t DecayReleaseRate[16]={6,24,48,72,114,168,204,240,300,750,1500,2400,3000,9000,15000,24000};
	
static uint8_t output;
static Sid_t Sid;
static Oscillator_t osc[OSCILLATORS];

void initialize()
{
	// TIMER1 used to generate sample and ms interrupts
	// TIMER1: Normal Mode
	// TIMER2: no prescaling
	TCCR1A = 0; 
	TCCR1B = (1 << CS10);	
	// TIMER1: set compare value to generate a 16kHz sample rate
	OCR1A = SAMPLERATECOUNT;
	// TIMER2: set compare value to generate a ms interrupt (1kHz)
	OCR1B = MSCOUNT;
		
#if defined(__AVR_ATmega8__)|| defined(__AVR_ATmega128__)
	// for FAST PWM on OC2 
	TCCR2 =  (1 << WGM21) | (1 << WGM20) | (1 << COM21) ;
	// tmr2 running on MCU full speed clock
	TCCR2 |= (1 << CS20);	
	// interrupt mask register: enable timer2 overflow
	TIMSK = (1 << TOIE2);
	
	// interrupt mask register: enable timer1 OCR1A and OCR1B interrupts	
	TIMSK |= (1 << OCIE1B) | (1 << OCIE1A);	
#else
	// for FAST PWM on OC2A 
	TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2A1) ;
	// tmr2 running on MCU full speed clock 
	TCCR2B = (1 << CS20);	
	// interrupt mask register: enable timer2 overflow
	TIMSK2 = (1 << TOIE2);
	
	// interrupt mask register: enable timer1 OCR1A and OCR1B interrupts	
	TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B);
#endif
}

static int8_t wave(Voice_t *voice, uint16_t phase)
{
	int8_t out;
	uint8_t n = phase >> 8;
	uint8_t wavetype = voice->ControlReg;
	
	if(wavetype & SAWTOOTH)  
	{
		out = n - 128;
	}

	if(wavetype & TRIANGLE) 
	{
		if(n&0x80) 
			out = ((n^0xFF)<<1)-128;
		else 
			out = (n<<1)-128;
	}

	if(wavetype & RECTANGLE) 
	{
		if(n > (voice->PW >> 4)) // SID has 12Bit pwm, here we use only 8Bit
			out = 127;
		else 
			out = -127;
	}
		
	return out;
}

static void waveforms()
{
	static uint16_t phase0,phase1,phase2;
	static int16_t temp,temp1;
	static uint8_t k;
	static uint16_t noise = 0xACE1;
	static uint8_t noise8;
	static uint16_t sig0,sig1,sig2;
	static uint16_t tempphase;

	// noise generator based on Galois LFSR
	noise = (noise >> 1) ^ (-(noise & 1) & 0xB400u);	
	noise8 = noise>>8;
	
	// Voice1
	tempphase=phase0+osc[0].freq_coefficient; //0.88us
	if(Sid.block.voice[0].ControlReg&NOISE)
	{				
		if((tempphase^phase0)&0x4000) sig0=noise8*osc[0].envelope;			
	}
	else
	{
		if(Sid.block.voice[0].ControlReg&RINGMOD)
		{				
			if(phase2&0x8000) sig0=osc[0].envelope*-wave(&Sid.block.voice[0],phase0);
			else sig0=osc[0].envelope*wave(&Sid.block.voice[0],phase0);
		}
		else sig0=osc[0].envelope*wave(&Sid.block.voice[0],phase0); //2.07us
	}
	phase0=tempphase;

	// Voice2
	tempphase=phase1+osc[1].freq_coefficient; //0.88us
	if(Sid.block.voice[1].ControlReg&NOISE)
	{				
		if((tempphase^phase1)&0x4000) sig1=noise8*osc[1].envelope;
	}
	else
	{
		if(Sid.block.voice[1].ControlReg&RINGMOD)
		{				
			if(phase0&0x8000) sig1=osc[1].envelope*-wave(&Sid.block.voice[1],phase1);
			else sig1=osc[1].envelope*wave(&Sid.block.voice[1],phase1);
		}
		else sig1=osc[1].envelope*wave(&Sid.block.voice[1],phase1); //2.07us
	}
	phase1=tempphase;

	// Voice3
	tempphase=phase2+osc[2].freq_coefficient; //0.88us
	if(Sid.block.voice[2].ControlReg&NOISE)
	{				
		if((tempphase^phase2)&0x4000) sig2=noise8*osc[2].envelope;
	}
	else
	{
		if(Sid.block.voice[2].ControlReg&RINGMOD)
		{				
			if(phase1&0x8000) sig2=osc[2].envelope*-wave(&Sid.block.voice[2],phase2);
			else sig2=osc[2].envelope*wave(&Sid.block.voice[2],phase2);
		}
		else sig2=osc[2].envelope*wave(&Sid.block.voice[2],phase2); //2.07us
	}
	phase2=tempphase;
	
	// voice filter selection
	temp=0; // direct output variable
	temp1=0; // filter output variable
	if(Sid.block.RES_Filt&FILT1) temp1+=sig0;
	else temp+=sig0;
	if(Sid.block.RES_Filt&FILT2) temp1+=sig1;
	else temp+=sig1;
	if(Sid.block.RES_Filt&FILT3) temp1+=sig2;
	else if(!(Sid.block.Mode_Vol&VOICE3OFF))temp+=sig2; // voice 3 with special turn off bit

	//filterOutput = IIR2((struct IIR_filter*)&filter04_06, filterInput);
	//IIR2(filter04_06, temp1);
	k=(temp>>8)+128;
	k+=temp1>>10; // no real filter implemeted yet
	
	output = k; // Output to PWM
}


static void envelopes()
{
	uint8_t n;
	uint8_t controll_regadr[3]={4,11,18};
	// if gate is ONE then the attack,decay,sustain cycle begins
	// if gate switches to zero the sound decays
	for(n=0;n<OSCILLATORS;n++)
	{
		if(Sid.sidregister[controll_regadr[n]]&GATE) // if gate set then attack,decay,sustain
		{
			if(osc[n].attackdecay_flag) 
			{	// if attack cycle
				osc[n].amp+=osc[n].m_attack;
				if(osc[n].amp>MAXLEVEL)
				{
					osc[n].amp=MAXLEVEL;
					osc[n].attackdecay_flag=false; // if level reached, then switch to decay
				}
			}
			else // decay cycle
			{
				if(osc[n].amp>osc[n].level_sustain)
				{
					osc[n].amp-=osc[n].m_decay;
					if(osc[n].amp<osc[n].level_sustain) osc[n].amp=osc[n].level_sustain;
				}

			} 
		}
		else // if gate flag is not set then release
		{
			osc[n].attackdecay_flag=true; // at next attack/decay cycle start wiht attack
			if(osc[n].amp>0)
			{
				osc[n].amp-=osc[n].m_release;
				if(osc[n].amp<0) osc[n].amp=0;
			}			
		}
		osc[n].envelope=osc[n].amp>>8;
	}
}

/************************************************************************

	interrupt routine timer 1 16kHz
	- calculate waverform phases
	- calculate waveforms
	
************************************************************************/
SIGNAL (SIG_OUTPUT_COMPARE1A)
{
	OCR1A += SAMPLERATECOUNT; // Reload timer
	waveforms(); //~22us
}

/************************************************************************
	
	interrupt routine timer 1 1kHz
	- calculate attack decay release
	
************************************************************************/
SIGNAL (SIG_OUTPUT_COMPARE1B)
{
	OCR1B += MSCOUNT; // Reload timer
	envelopes(); //~16us
}

/************************************************************************

	interrupt routine timer 2 overflow 
	- set PWM output
	
************************************************************************/
SIGNAL (SIG_OVERFLOW2)
{
#if defined(__AVR_ATmega8__)|| defined(__AVR_ATmega128__)
	OCR2 = output; // Output to PWM
#else
	OCR2A = output; // Output to PWM
#endif	
}

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

void SID::begin()
{
	// TIMER2: set OC2(A) to output
	pinMode(11, OUTPUT);
	initialize();
		
	//initialize SID-registers	
	Sid.sidregister[6]=0xF0;
	Sid.sidregister[13]=0xF0;
	Sid.sidregister[20]=0xF0;

	
	// set all amplitudes to zero
	for(int n=0;n<OSCILLATORS;n++) {
		osc[n].attackdecay_flag=true;
		setenvelope(&Sid.block.voice[n]);
		osc[n].amp=0;
	}
}


// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries
/************************************************************************
	
	uint8_t set_sidregister(uint8_t regnum, uint8_t value)

	The registers of the virtual SID are set by this routine.
	For some registers it is necessary to transform the SID-register
	values to some internal settings of the emulator. 
	To select this registers and to start the calculation, the switch/
	case statement is used.
	For instance: If setting the SID envelope register, new attach, decay
	sustain times are calculated.

	4.2007 ch

************************************************************************/
uint8_t SID::set_register(uint8_t regnum, uint8_t value)
{
	if(regnum>NUMREGISTERS-1) 
		return 1;
		
	Sid.sidregister[regnum]=value;

	switch(regnum)
	{
		//voice1
		case 1:
			osc[0].freq_coefficient=(Sid.sidregister[0]+(Sid.sidregister[1]<<8))>>2;
			break;
		case 5: setenvelope(&Sid.block.voice[0]);break;
		case 6: setenvelope(&Sid.block.voice[0]);break;
		
		//voice2
		case 8:
			osc[1].freq_coefficient=(Sid.sidregister[7]+(Sid.sidregister[8]<<8))>>2;
			break;
		case 12: setenvelope(&Sid.block.voice[1]);break;
		case 13: setenvelope(&Sid.block.voice[1]);break;		
		
		//voice3
		case 15:
			osc[2].freq_coefficient=(Sid.sidregister[14]+(Sid.sidregister[15]<<8))>>2;
			break;
		case 19: setenvelope(&Sid.block.voice[2]);break;
		case 20: setenvelope(&Sid.block.voice[2]);break;			
	}	
	return 0;
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

uint8_t SID::get_wavenum(Voice_t *voice)
{
	uint8_t n;

	if(voice==&Sid.block.voice[0]) n=0;
	if(voice==&Sid.block.voice[1]) n=1;
	if(voice==&Sid.block.voice[2]) n=2;
	
	return n;
}

void SID::setfreq(Voice_t *voice,uint16_t freq)
{
	uint32_t templong;
	uint8_t n;

	n=get_wavenum(voice);
	
	templong=freq;
	osc[n].freq_coefficient=templong*4000/SAMPLEFREQ;
}


void SID::setenvelope(Voice_t *voice)
{
	uint8_t n;
	
	n=get_wavenum(voice);
	osc[n].attackdecay_flag=true;

	osc[n].level_sustain=(voice->SustainRelease>>4)*SUSTAINFACTOR;
	osc[n].m_attack=MAXLEVEL/AttackRate[voice->AttackDecay>>4];
	osc[n].m_decay=(MAXLEVEL-osc[n].level_sustain*SUSTAINFACTOR)/DecayReleaseRate[voice->AttackDecay&0x0F];
	osc[n].m_release=(osc[n].level_sustain)/DecayReleaseRate[voice->SustainRelease&0x0F];
}


