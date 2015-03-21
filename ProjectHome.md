This Arduino library emulates the SID sound chip of the famous Commodore 64 home computer.

It is based on the [Atmega8 MOS6581 SID Emulator](http://www.roboterclub-freiburg.de/atmega_sound/atmegaSID.html) by Christoph Haberer, but replaces the DSS tables with online functions to save memory and allow its use as a library.

Unlike the original emulator, this library does not require any additional wiring. The sound is output through the digital **Pin 9** of the Arduino board. A speaker can be directly connected between ground and this pin but the best results are obtained if a simple low-pass filter and an amplifier are used.