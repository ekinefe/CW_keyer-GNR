#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "GlobalVars.h"
#include "Buzzer_LED_Controller.h"

String getMorseCode(char c) {
  if (c >= 'A' && c <= 'Z') return morseTable[c - 'A'];
  if (c >= '0' && c <= '9') return morseTable[c - '0' + 26];
  if (c == ' ') return " ";
  return "";
}

// Ensure "startEncoding" is available to be called from .ino
void startEncoding(String text) {
    encodeBuffer = text;
    encodeBuffer.trim();
    encodeBuffer.toUpperCase();
    if (encodeBuffer.length() > 0) {
        charIndex = 0;
        symbolIndex = 0;
        encodeState = INTER_LETTER; 
        encodeStartTime = millis();
        // Serial.print("Encoding: "); Serial.println(encodeBuffer); // CLI handles echo if needed
    }
}

void handleEncoding(unsigned long now) {
  static unsigned long elementStart = 0;
  static bool isPlaying = false;
  static char symbol = '\0';
  
  // REMOVED Serial.available logic loop. State machine only.

  if (encodeState == IDLE_ENCODE) return;

  if (charIndex >= encodeBuffer.length()) {
    encodeState = IDLE_ENCODE;
    Serial.print(F("Encoded: "));
    Serial.println(encodeBuffer);
    Serial.println(F(" [Done]"));
    return;
  }

  char currentChar = encodeBuffer.charAt(charIndex);
  
  switch (encodeState) {
    case INTER_LETTER:
       if (now - encodeStartTime >= dotDuration * 3) {
         if (currentChar == ' ') {
            encodeState = WORD_GAP;
            encodeStartTime = now;
         } else {
            currentMorse = getMorseCode(currentChar);
            if (currentMorse == "") { 
                charIndex++;
                encodeStartTime = now; 
            } else {
                symbolIndex = 0;
                encodeState = ELEMENT_PLAY;
                encodeStartTime = now;
            }
         }
       }
       break;

    case WORD_GAP:
        if (now - encodeStartTime >= dotDuration * 7) {
            charIndex++;
            encodeState = INTER_LETTER;
            encodeStartTime = now;
        }
        break;

    case ELEMENT_PLAY:
        if (symbolIndex < currentMorse.length()) {
            symbol = currentMorse.charAt(symbolIndex);
            if (symbol == '.') {
                playDot();
                keyingLength = dotDuration;
            } else if (symbol == '-') {
                playDash();
                keyingLength = dotDuration * 3;
            }
            elementStart = now;
            encodeState = INTER_ELEMENT;
            isPlaying = true;
        } else {
            charIndex++;
            encodeState = INTER_LETTER;
            encodeStartTime = now;
        }
        break;

    case INTER_ELEMENT:
        if (isPlaying && now - elementStart >= keyingLength) {
            stopTone();
            isPlaying = false;
            elementStart = now;
        } else if (!isPlaying && now - elementStart >= dotDuration) {
            symbolIndex++;
            encodeState = ELEMENT_PLAY; 
        }
        break;
  }
}

#endif
