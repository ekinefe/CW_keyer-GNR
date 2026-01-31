#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include "PinSettings.h"
#include "GlobalVars.h"
#include "LCD_Controller.h"

// Forward Declaration to prevent Scope Error
void startEncoding(String text);

// Button states
int lastButtonState = 0;
unsigned long lastButtonDebounceTime = 0;
const int debounceDelay = 50;

int readAnalogueButtons() {
    analogRead(FunctionButtons); // Dummy read
    int val = analogRead(FunctionButtons);
    
    // DEBUG: Print value if button indicates press (value drops significantly below 1023)
    // if (val < 1000) {
    //     Serial.print("DEBUG ANALOG: ");
    //     Serial.println(val);
    // }

    if (val > 1000) return 0; // Open Circuit (with PULLUP it should be ~1023)
    
    // Standard "Resistor Ladder" Logic (To be calibrated)
    // We try broader ranges first, but the Debug Output is key.
    if (val < 50) return 1;
    if (val < 200) return 2;
    if (val < 400) return 3;
    if (val < 650) return 4;
    return 5;
}

void performButtonAction(int btn) {
    if (btn == 0) return;
    
    // Log to Serial (App picks this up)
    Serial.print("\nAction: F"); Serial.print(btn); Serial.println(" Pressed");
    
    // Actions with Custom LCD Overlays
    if (btn == 1) { // F1: CQ
        showOverlay("Function F1", "Sending CQ");
        startEncoding("CQ");
        // mute the buzzer
    }
    else if (btn == 2) { // F2: Mute Toggle
        silentMode = !silentMode;
        if (silentMode) {
             showOverlay("Audio", "MUTED");
             Serial.println("\nAction: Audio MUTED");
        } else {
             showOverlay("Audio", "ENABLED");
             Serial.println("\nAction: Audio ENABLED");
        }
    }
    else if (btn == 3) { // F3: Cycle Tone
         showOverlay("Function F3", "NOT PROGRAMMED");
         Serial.println("\nNOT PROGRAMMED");
    }
    else if (btn == 4) { // F4: Info
         showOverlay("Function F4", "NOT PROGRAMMED");
         Serial.println("\nNOT PROGRAMMED");
    }
    else if (btn == 5) { // F5: Status
         showOverlay("Function F5", "NOT PROGRAMMED");
         Serial.println("\nNOT PROGRAMMED");
    }
}

void checkButtons() {
    static unsigned long lastModeSwitchTime = 0;
    static unsigned long lastCleanTime = 0;
    unsigned long now = millis();

    // Mode Button
    if (digitalRead(MODEButton) == LOW && (now - lastModeSwitchTime > 300)) {
        decoderMode = (decoderMode + 1) % 3;
        lastModeSwitchTime = now;
        
        String mLabel = "CHAR";
        if (decoderMode == 1) mLabel = "MORSE";
        if (decoderMode == 2) mLabel = "BOTH";
        
        showOverlay("Decoder Mode", mLabel);
        Serial.print("\nMode set to "); Serial.println(mLabel);
    }

    // Clean Button
    if (digitalRead(CLEANButton) == LOW && (now - lastCleanTime > 300)) {
        morseBuffer = "";
        sentenceBuffer = "";
        encodeBuffer = "";
        currentMorse = "";
        
        clearLCDText();
        showOverlay("Buffer", "CLEARED");
        Serial.println("\nAction: Buffer Cleared");
        lastCleanTime = now;
    }
    
    // Analog Function Buttons (Debounced)
    int reading = readAnalogueButtons();
    if (reading != lastButtonState) {
        lastButtonDebounceTime = now;
    }
    
    if ((now - lastButtonDebounceTime) > debounceDelay) {
        static int currentStableState = 0;
        if (reading != currentStableState) {
            currentStableState = reading;
            if (currentStableState != 0) {
                performButtonAction(currentStableState);
            }
        }
    }
    lastButtonState = reading;
}

#endif
