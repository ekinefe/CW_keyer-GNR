#ifndef CLI_CONTROLLER_H
#define CLI_CONTROLLER_H

#include <Arduino.h>
#include "GlobalVars.h"
#include "LCD_Controller.h"

// Returns true if the string was a command and handled
// Returns false if it should be treated as text to encode
bool processCLI(String input) {
    input.trim();
    if (input.length() == 0) return true; // Ignore empty lines

    // Convert to lower case for command matching
    String lowerInput = input;
    lowerInput.toLowerCase();

    if (lowerInput == "help") {
        Serial.println(F("--- CW Keyer CLI ---"));
        Serial.println(F("Commands:"));
        Serial.println(F("  wpm <val>   : Set WPM (will be overridden if pot moves)"));
        Serial.println(F("  tone <val>  : Set Tone Hz (will be overridden if pot moves)"));
        Serial.println(F("  mode <0-2>  : Set Mode (0=CHAR, 1=MRS, 2=BOTH)"));
        Serial.println(F("  silent <0|1>: Set Silent Mode (0=OFF, 1=ON)"));
        Serial.println(F("  clear       : Clear buffers"));
        Serial.println(F("  status      : Show current state"));
        Serial.println(F("  <text>      : Anything else is sent as Morse"));
        return true;
    }

    if (lowerInput.startsWith("wpm ")) {
        int val = input.substring(4).toInt();
        if (val >= 5 && val <= 50) {
            WPM = val;
            dotDuration = 1200 / WPM;
            lastWPM = WPM; // Update lastWPM so pot doesn't immediately revert unless moved
            Serial.print(F("WPM set to ")); Serial.println(WPM);
            updateLCDToneHz(toneHz);
        } else {
            Serial.println(F("Invalid WPM (5-50)"));
        }
        return true;
    }

    if (lowerInput.startsWith("tone ")) {
        int val = input.substring(5).toInt();
        if (val >= minHz && val <= maxHz) {
            toneHz = val;
            lastToneHz = val;
            Serial.print(F("Tone set to ")); Serial.println(toneHz);
            updateLCDToneHz(toneHz);
        } else {
            Serial.println(F("Invalid Tone Hz"));
        }
        return true;
    }

    if (lowerInput.startsWith("mode ")) {
        int val = input.substring(5).toInt();
        if (val >= 0 && val <= 2) {
            decoderMode = val;
            Serial.print(F("Mode set to ")); Serial.println(val);
            updateLCDToneHz(toneHz);
        } else {
            Serial.println(F("Invalid Mode (0-2)"));
        }
        return true;
    }

    if (lowerInput.startsWith("silent ")) {
        int val = input.substring(7).toInt();
        silentMode = (val == 1);
        Serial.print(F("Silent Mode: ")); Serial.println(silentMode ? "ON" : "OFF");
        return true;
    }

    if (lowerInput == "clear") {
        morseBuffer = "";
        sentenceBuffer = "";
        encodeBuffer = "";
        currentMorse = "";
        clearLCDText();
        // Serial.println(F("Buffers cleared"));
        Serial.println(F("\nBuffers cleared"));
        return true;
    }

    if (lowerInput == "status") {
        Serial.print(F("WPM: ")); Serial.println(WPM);
        Serial.print(F("Tone: ")); Serial.println(toneHz);
        Serial.print(F("Mode: ")); Serial.println(decoderMode);
        Serial.print(F("Sentence: ")); Serial.println(sentenceBuffer);
        return true;
    }

    // Not a command
    return false;
}

#endif
