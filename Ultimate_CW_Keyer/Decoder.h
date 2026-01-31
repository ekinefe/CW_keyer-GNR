#ifndef DECODER_H
#define DECODER_H

#include "GlobalVars.h"
#include <Arduino.h>

char checkDecoderTimeout(unsigned long now) {
    // Check if we assume the character is finished (3x dot duration silence)
    // but only if we have something in the buffer
    static unsigned long lastCheck = 0;
    // ...
    // Note: The original logic in .ino was embedded in loop. 
    // I will extract the core decode logic here.
    return 0; 
}

// Helper to look up char from morse string
char decodeChar(String sequence) {
    for (int i = 0; i < 36; i++) {
        if (String(morseTable[i]) == sequence) {
            if (i < 26) return (char)('A' + i);
            else return (char)('0' + (i - 26));
        }
    }
    return '?';
}

void decodeMorse(String sequence) {
   char c = decodeChar(sequence);
   String toAdd = "";

   if (decoderMode == 0) { // CHAR
       if (c != '?') toAdd += c;
       else toAdd += "?";
   } 
   else if (decoderMode == 1) { // MRS (Morse)
       toAdd += sequence;
       toAdd += " "; // Space between morse sequences
   } 
   else if (decoderMode == 2) { // BOTH
       if (c != '?') {
           toAdd += c;
           toAdd += "[";
           toAdd += sequence;
           toAdd += "] ";
       } else {
           toAdd += "?";
       }
   }
   
   Serial.print(toAdd);
   sentenceBuffer += toAdd;
}

#endif
