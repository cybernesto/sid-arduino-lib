/*

<!> Programmer = USBasp for arduino Nano ATmega328

 Brian Tucker 1-5-2015
 sid-arduino-lib:
 https://code.google.com/p/sid-arduino-lib/
 based SID player example program sets
 the 25 SID/6581 registers at 50Hz (delay(19)=~50Hz) 
 The register data is created using the excellent app
 "SIDDumper" from the SIDCog project:
 SIDCog:
 http://forums.parallax.com/showthread.php/118285-SIDcog-The-sound-of-the-Commodore-64-!-%28Now-in-the-OBEX%29
 SIDDumper:
 http://gadgetgangster.com/scripts/displayasset.php?id=361
 For best results use an IRQ at 50 or 60Hz and set the registers programatically.
 For info on how to make music with the SID chip and related registers
 I recomend the e-magazine "Commodore Hacking":
 http://www.ffd2.com/fridge/chacking/
 Specifically the Rob Hubbard play subroutine as that is the code
 all serious C64 SID musicians must understand.
 
 convert the dump to number with : http://www.expertsetup.com/~brian/c64/JConverter.jar / http://softcollection.sytes.net/javaprog
 
*/

// Connect PIN 9 from your arduino Uno to audio output (+ ground to the other pin of the audio output)!

#include <avr/pgmspace.h>  //used to store data in the flash rom
//#include "Alternative_Fuel_dmp.h"       //RAW SID register data file in flash
//#include "Castlevania_64_Mixes_dmp.h"       //RAW SID register data file in flash
//#include "Zelda_1_Dungeon_dmp.h"       //RAW SID register data file in flash
//#include "Open_Groove_dmp.h"
//#include "Vibralux_dmp.h"
//#include "Visitors_dmp.h"
#include "marche_hiver_dmp.h"
//#include "/home/forgeot/ownCloud/arduino/sketches/_musique/sidTest/Commando.dmp.h"
//#include "/home/path/ownCloud/arduino/sketches/_musique/sidTest/Commando.dmp.h"

#include <SID.h>           //
SID mySid;                 //

#define LED 13

void setup()  { 
  mySid.begin();
  digitalWrite(LED,HIGH);
      } 

void loop() {
  for(uint16_t sidPointer=0;sidPointer<=sidLength;sidPointer++){
    for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, (pgm_read_byte(&sidData[(sidPointer+sidRegister)])));
    };
      //digitalWrite(LED,HIGH);
    delay(19);
      //digitalWrite(LED,LOW);
    sidPointer=sidPointer+24;
  };
}

