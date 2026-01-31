#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <Arduino.h>

// ----------------------------------------------------------------------------
// GLOBAL VARIABLE DECLARATIONS
// ----------------------------------------------------------------------------

// Use 'extern' here, define them in Ultimate_CW_Keyer.ino

// State Machine for Serial-to-Morse
enum EncodeState {
    IDLE_ENCODE,    // Waiting for new text to encode
    ELEMENT_PLAY,   // Playing a dot or dash
    INTER_ELEMENT,  // Gap between elements in a character
    INTER_LETTER,   // Gap between letters
    WORD_GAP        // Gap between words
};

extern EncodeState encodeState;
extern String encodeBuffer;
extern int charIndex;
extern int symbolIndex;
extern unsigned long encodeStartTime;
extern String currentMorse;

// Timing & WPM
extern int WPM;
extern int dotDuration;
extern int lastWPM;

// Decoder / Paddle Logic
extern String morseBuffer;
extern String sentenceBuffer;
extern bool lastWasDot;
extern bool justDecoded;
extern int decoderMode; // 0=CHAR, 1=MRS, 2=BOTH
extern int keyingLength;

// Audio
extern int toneHz;
extern int lastToneHz;
extern const int minHz;
extern const int maxHz;

// Silent Mode
extern bool silentMode;

// Morse Table
extern const char* morseTable[36];

#endif
