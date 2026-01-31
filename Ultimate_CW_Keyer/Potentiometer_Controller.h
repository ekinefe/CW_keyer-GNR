#ifndef POTENTIOMETER_CONTROLLER_H
#define POTENTIOMETER_CONTROLLER_H

#include <Arduino.h>
#include "PinSettings.h"
#include "GlobalVars.h"
#include "LCD_Controller.h" 

void checkPotentiometers() {
    static long smoothWPMVal = -1;
    static long smoothToneVal = -1;

    // --- WPM ---
    int rawWPM = analogRead(WPMPotPin);
    if (smoothWPMVal == -1) smoothWPMVal = rawWPM; // Init
    else smoothWPMVal = (smoothWPMVal * 15 + rawWPM) / 16; // Simple smoothing

    // map 0-1023 to 5-45 WPM
    int newWPM = map(smoothWPMVal, 0, 1023, 5, 45); 
    
    // Add hysteresis to prevent flickering
    if (abs(newWPM - lastWPM) >= 1) {
        WPM = newWPM;
        dotDuration = 1200 / WPM;
        lastWPM = WPM;
        triggerWPMOverlay(WPM); 
    }

    // --- TONE ---
    int rawTone = analogRead(TONEPotPin);
    if (smoothToneVal == -1) smoothToneVal = rawTone;
    else smoothToneVal = (smoothToneVal * 15 + rawTone) / 16;

    int newHz = map(smoothToneVal, 0, 1023, minHz, maxHz);
    
    // Hysteresis of ~15Hz
    if (abs(newHz - lastToneHz) > 15) {
        toneHz = newHz;
        lastToneHz = newHz;
        triggerHzOverlay(toneHz);
    }
}

#endif
