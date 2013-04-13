MOS6581 SID Emulator Arduino Library

This library emulates the SID sound chip of the famous Commodore 64.
It is based on the Atmega8 MOS6581 SID Emulator from Christoph Haberer.
Library conversion and memory optimization by Mario Patino.

Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:

Arduino/lib/targets/libraries/SID              (this library's folder)
Arduino/lib/targets/libraries/SID/SID.cpp      (the library implementation file)
Arduino/lib/targets/libraries/SID/SID.h        (the library description file)
Arduino/lib/targets/libraries/SID/keywords.txt (the syntax coloring file)
Arduino/lib/targets/libraries/SID/examples     (the examples in the "open" menu)
Arduino/lib/targets/libraries/SID/readme.txt   (this file)

Building
--------------------------------------------------------------------------------

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select SID.  This will add a corresponding line to the top of your sketch:
#include <SID.h>

To stop using this library, delete that line from your sketch.


