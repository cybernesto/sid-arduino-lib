namespace MIDI
{
/** 440hz = A4 = 0x1CD6
 * is normally used to tune instruments.
 * MIDI spec is 127 note wide, whereas SID has 94 possibilites.
 * I rescaled the two chords, putting zero where SID cannot cope 
 */

const uint16_t midi2sidMapper[]={
  0,// MIDI 0 Note C-2 (OUT-OF-RANGE)
  0,// MIDI 1 Note C#-2
  0,// MIDI 2 Note 
  0,// MIDI 3 Note 
  0,// MIDI 4 Note 
  0,// MIDI 5 Note 
  0,// MIDI 6 Note 
  0,// MIDI 7 Note 
  0,// MIDI 8 Note 
  0,// MIDI 9 Note 
  0,// MIDI 10 Note 
  0,// MIDI 11 Note 
  0,// MIDI 12 Note 
  0,// MIDI 13 Note 
  0,// MIDI 14 Note 
  0,// MIDI 15 Note 
  0,// MIDI 16 Note 
  0,// MIDI 17 Note 
  0,// MIDI 18 Note 
  0,// MIDI 19 Note 
  0,// MIDI 20 Note 
  0,// MIDI 21 Note 
  0,// MIDI 22 Note 
  0,// MIDI 23 Note 
  0,// MIDI 24 Note C0
  0x0123,// MIDI 25 Note C0#
  0x0134,// MIDI 26 Note D0
  0x0146,// MIDI 27 Note D0#
  0x015A,// MIDI 28 Note E0
  0x016E,// MIDI 29 Note F0
  0x0184,// MIDI 30 Note F0#
  0x019B,// MIDI 31 Note G0
  0x01B3,// MIDI 32 Note G0#
  0x01CD,// MIDI 33 Note A0
  0x01E9,// MIDI 34 Note A0#
  0x0206,// MIDI 35 Note B0
  0x0225,// MIDI 36 Note C1
  0x0245,// MIDI 37 Note C1#
  0x0268,// MIDI 38 Note D1
  0x028C,// MIDI 39 Note D1#
  0x02B3,// MIDI 40 Note E1
  0x02DC,// MIDI 41 Note F1
  0x0308,// MIDI 42 Note F1#
  0x0336,// MIDI 43 Note G1
  0x0367,// MIDI 44 Note G1#
  0x039B,// MIDI 45 Note A1
  0x03D2,// MIDI 46 Note A1#
  0x040C,// MIDI 47 Note B1
  0x0449,// MIDI 48 Note C2
  0x048B,// MIDI 49 Note C2#
  0x04D0,// MIDI 50 Note D2
  0x0519,// MIDI 51 Note D2#
  0x0567,// MIDI 52 Note E2
  0x05B9,// MIDI 53 Note F2
  0x0610,// MIDI 54 Note F2#
  0x066C,// MIDI 55 Note G2
  0x06CE,// MIDI 56 Note G2#
  0x0735,// MIDI 57 Note A2
  0x07A3,// MIDI 58 Note A2#
  0x0817,// MIDI 59 Note B2
  0x0893,// MIDI 60 Note C3
  0x0915,// MIDI 61 Note C3#
  0x099F,// MIDI 62 Note D3
  0x0A32,// MIDI 63 Note D3#
  0x0ACD,// MIDI 64 Note E3
  0x0B72,// MIDI 65 Note F3
  0x0C20,// MIDI 66 Note F3#
  0x0CD8,// MIDI 67 Note G3
  0x0D9C,// MIDI 68 Note G3#
  0x0E6B,// MIDI 69 Note A3
  0x0F46,// MIDI 70 Note A3#
  0x102F,// MIDI 71 Note B3
  0x1125,// MIDI 72 Note C4   MISSED FROM A LOT OF OCRization of original manual!
  0x122A,// MIDI 73 Note C4#
  0x133F,// MIDI 74 Note D4
  0x1464,// MIDI 75 Note D4#
  0x159A,// MIDI 76 Note E4
  0x16E3,// MIDI 77 Note F4
  0x183F,// MIDI 78 Note F4#
  0x19B1,// MIDI 79 Note G4
  0x1B38,// MIDI 80 Note G4#
  0x1CD6,// MIDI 81 Note A4
  0x1E8D,// MIDI 82 Note A4#
  0x205E,// MIDI 83 Note B4
  0x224B,// MIDI 84 Note C5
  0x2455,// MIDI 85 Note C5#
  0x267E,// MIDI 86 Note D5
  0x28C8,// MIDI 87 Note D5#
  0x2B34,// MIDI 88 Note E5
  0x2DC6,// MIDI 89 Note F5
  0x307F,// MIDI 90 Note F5#
  0x3361,// MIDI 91 Note G5
  0x366F,// MIDI 92 Note G5#
  0x39AC,// MIDI 93 Note A5
  0x3D1A,// MIDI 94 Note A5#
  0x40BC,// MIDI 95 Note B5
  0x4495,// MIDI 96 Note C6
  0x48A9,// MIDI 97 Note C6#
  0x4CFD,// MIDI 98 Note D6
  0x51A1,// MIDI 99 Note D6#
  0x5669,// MIDI 100 Note E6
  0x5B8C,// MIDI 101 Note F6
  0x60FE,// MIDI 102 Note F6#
  0x66C2,// MIDI 103 Note G6
  0x6CDF,// MIDI 104 Note G6#
  0x7358,// MIDI 105 Note A6
  0x7A02,// MIDI 106 Note A6#
  0x8178,// MIDI 107 Note B6
  0x892B,// MIDI 108 Note C7
  0x9153,// MIDI 109 Note C7#
  0x99F7,// MIDI 110 Note D7
  0xA31F,// MIDI 111 Note D7#
  0xACD2,// MIDI 112 Note E7
  0xB719,// MIDI 113 Note F7
  0xC1FC,// MIDI 114 Note F7#
  0xCD85,// MIDI 115 Note G7
  0xD9BD,// MIDI 116 Note G7#
  0xE6B0,// MIDI 117 Note A7
  0xF467,// MIDI 118 Note A7#
  0, // OUT_FOR_RANGE MIDI 120
  0, // OUT_FOR_RANGE
  0, // OUT_FOR_RANGE
  0, // OUT_FOR_RANGE
  0, // OUT_FOR_RANGE
  0, // OUT_FOR_RANGE
  0, // OUT_FOR_RANGE
  0, // OUT_FOR_RANGE
  0 // OUT_FOR_RANGE MIDI 127
};


 
const uint8_t nC0d=25;
const uint8_t nD0=26;
const uint8_t nD0d=27;
const uint8_t nE0=28;
const uint8_t nF0=29;
const uint8_t nF0d=30;
const uint8_t nG0=31;
const uint8_t nG0d=32;
const uint8_t nA0=33;
const uint8_t nA0d=34;
const uint8_t nB0=35;
const uint8_t nC1=36;
const uint8_t nC1d=37;
const uint8_t nD1=38;
const uint8_t nD1d=39;
const uint8_t nE1=40;
const uint8_t nF1=41;
const uint8_t nF1d=42;
const uint8_t nG1=43;
const uint8_t nG1d=44;
const uint8_t nA1=45;
const uint8_t nA1d=46;
const uint8_t nB1=47;
const uint8_t nC2=48;
const uint8_t nC2d=49;
const uint8_t nD2=50;
const uint8_t nD2d=51;
const uint8_t nE2=52;
const uint8_t nF2=53;
const uint8_t nF2d=54;
const uint8_t nG2=55;
const uint8_t nG2d=56;
const uint8_t nA2=57;
const uint8_t nA2d=58;
const uint8_t nB2=59;
const uint8_t nC3=60;
const uint8_t nC3d=61;
const uint8_t nD3=62;
const uint8_t nD3d=63;
const uint8_t nE3=64;
const uint8_t nF3=65;
const uint8_t nF3d=66;
const uint8_t nG3=67;
const uint8_t nG3d=68;
const uint8_t nA3=69;
const uint8_t nA3d=70;
const uint8_t nB3=71;
const uint8_t nC4=72;
const uint8_t nC4d=73;
const uint8_t nD4=74;
const uint8_t nD4d=75;
const uint8_t nE4=76;
const uint8_t nF4=77;
const uint8_t nF4d=78;
const uint8_t nG4=79;
const uint8_t nG4d=80;
const uint8_t nA4=81;
const uint8_t nA4d=82;
const uint8_t nB4=83;
const uint8_t nC5=84;
const uint8_t nC5d=85;
const uint8_t nD5=86;
const uint8_t nD5d=87;
const uint8_t nE5=88;
const uint8_t nF5=89;
const uint8_t nF5d=90;
const uint8_t nG5=91;
const uint8_t nG5d=92;
const uint8_t nA5=93;
const uint8_t nA5d=94;
const uint8_t nB5=95;
const uint8_t nC6=96;
const uint8_t nC6d=97;
const uint8_t nD6=98;
const uint8_t nD6d=99;
const uint8_t nE6=100;
const uint8_t nF6=101;
const uint8_t nF6d=102;
const uint8_t nG6=103;
const uint8_t nG6d=104;
const uint8_t nA6=105;
const uint8_t nA6d=106;
const uint8_t nB6=107;
const uint8_t nC7=108;
const uint8_t nC7d=109;
const uint8_t nD7=110;
const uint8_t nD7d=111;
const uint8_t nE7=112;
const uint8_t nF7=113;
const uint8_t nF7d=114;
const uint8_t nG7=115;
const uint8_t nG7d=116;
const uint8_t nA7=117;
const uint8_t nA7d=118;
const uint8_t nB7=119;
};
