// Example of SID Commander: pushes data to SID 
// ATMega2560 suggested because can hold more data 
// Wire UNOA4 -->Mega20, UONA5-->Mega21
// Uno: TWI: A4 (SDA) pin and A5 (SCL) pin
// MEGA TWI: 20 (SDA) and 21 (SCL)
// then link grounds.
// The Commander did not depends on StereoSID library
// Created 16 April 2015
// This example code is in the public domain.

#include <Wire.h>

#define VOICE1	0
#define VOICE2	7
#define VOICE3	14


/** Enhance SID 
  *http://www.cplusplus.com/forum/beginner/18737/
  */
class RSID {
  public:
    void initI2CAsMaster(int device);
    uint8_t set_register(uint8_t regnum, uint8_t value);
    void playTestIntro();
    void loadPiano(uint8_t voice);
    void configVoice(uint8_t voice, uint8_t waveform, uint16_t pulsewidth /*12bit*/, uint8_t attackDecay, uint8_t sustainRelease, uint8_t releasePoint);
    uint16_t  midi2Sid(int midin);
    void play(uint8_t voice, uint16_t freq);
  protected:
    int device;
};

void RSID::initI2CAsMaster(int deviceI){
  Wire.begin(); // join i2c bus (address optional for master)
  device=deviceI;
}

uint8_t RSID::set_register(uint8_t regnum, uint8_t value){
  Wire.beginTransmission(device);
  Wire.write(regnum);Wire.write(value);
  Wire.endTransmission();
}

uint16_t  RSID::midi2Sid(int midin){
  // NOTE_D03 --> 131
  // C3    130.81	2195
  // 440hz = A4 = 0x1CD6
  // MIDI    A4  = 81
  // return midi2FreqOLD(midin)/0.0596;
  /*
 */
  switch(midin){
  case 60:return 0x122A;  // C4$	277.18	4650	122A 
  case 61:return 0x133F;  
  case 62:return 0x1464;  
  case 63:return 0x159A;
  case 64:return 0x16E3; 
  case 65:return 0x183F;  // FA
  case 66:return 0x1981;
  case 67:return 0x1B38;
  case 68:return 0x1CD6;
  case 69:return 0x1E80;
  case 70:return 0x205E;
  case 71:return 0x224B; // 
  case 72:return 0x2455; // C5$	554.37	9301	2455
  default:return 0x0;
    }
}


void RSID::play(uint8_t voice, uint16_t freq){
  byte low = freq & 0x00FF;
  byte high= freq >>8;
  set_register(voice+0,low);
  set_register(voice+1,high);
}

void RSID::playTestIntro() {
  /*loadPiano(VOICE1_Right);
  loadPiano(VOICE2_Left);
  loadPiano(VOICE3);*/
  
  set_register(24,15); // MAX VOLUME
  // play a tadalike
  play(VOICE1,midi2Sid(60 /*DO*/));
  delay(200);
  play(VOICE2,midi2Sid(62 /*RE*/));
  delay(200);
  play(VOICE1,0);
  play(VOICE2,0);
  // Hmmm Too voice seems distort on piezo...time to aplify better?
  play(VOICE1, midi2Sid(64 /*MI*/));
  delay(250);
  play(VOICE1,0);
  play(VOICE2, midi2Sid(60 /*DO*/));
  delay(250);  
  play(VOICE3,0); 
  play(VOICE2,0); 
  play(VOICE1,0); 
  
}

RSID myRemoteSID;

void setup()
{
  Serial.begin(9600); 
  myRemoteSID.initI2CAsMaster(23);
  Serial.println("Sid Master Ready");
}

void loop()
{
  Serial.println("Sid TEST");
  myRemoteSID.playTestIntro();
  delay(2000);
}

// Local variables:
// mode: c++
// mode: auto-complete
// End:
