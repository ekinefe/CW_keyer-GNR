#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include "PinSettings.h"
#include "GlobalVars.h"
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd; // Defined in .ino

unsigned long overlayStartTime = 0;
bool isOverlayActive = false;
const unsigned long OVERLAY_DURATION = 1500; // 1.5 seconds

// Forward declaration
void updateDefaultView();

// Custom WPM Icon
byte wpmLogo[8] = {
  B10101, // W (top points)
  B10101, // W (bottom points)
  B00000, // Spacer
  B11100, // P (loop)
  B10000, // P (stem)
  B00000, // Spacer
  B11011, // M (ears)
  B10101  // M (legs)
};

void lcdInit() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, wpmLogo); // Register custom char
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CW Keyer Ultimate");
  delay(1500);
  lcd.clear();
  updateDefaultView();
}

// Default View: "MODE:CHAR 20 WPM" (16 chars)
void updateDefaultView() {
    if (isOverlayActive) return; 

    lcd.setCursor(0, 0);
    lcd.print("MODE:");
    if (decoderMode == 0) lcd.print("CHAR");
    else if (decoderMode == 1) lcd.print("MRS ");
    else lcd.print("BOTH");

    // "ICON: XX"
    lcd.setCursor(13, 0); 
    lcd.write(1); // Print custom WPM icon
    // lcd.print(":");
    if (WPM < 10) lcd.print("0"); 
    lcd.print(WPM);
}

// Overlay function for temporary messages
void showOverlay(String line1, String line2) {
    overlayStartTime = millis();
    isOverlayActive = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

// Triggered by Pot change
void triggerWPMOverlay(int val) {
    showOverlay("WPM : " + String(val), "                ");
    Serial.print(F("WPM set to ")); Serial.println(val);
}

void triggerHzOverlay(int val) {
    showOverlay("HZ : " + String(val), "                ");
    Serial.print(F("Tone set to ")); Serial.println(val);
}

// Called every loop to check timeout
void lcdService() {
    if (isOverlayActive) {
        if (millis() - overlayStartTime > OVERLAY_DURATION) {
            isOverlayActive = false;
            lcd.clear();
            updateDefaultView();
            // Restore text buffer on line 1
            // Refactored updateLCDText below needs to be called
            extern void restoreLCDText(); // loop back
            restoreLCDText(); 
        }
    }
}

void updateLCDText(const String& textBuf) {
  // If overlay is active, do NOT write text unless we want to clobber it.
  // We generally want to ignore text updates during overlay
  if (isOverlayActive) return;

  lcd.setCursor(0, 1);
  String text = textBuf;
  if (text.length() > 16)
    text = text.substring(text.length() - 16);
  lcd.print(text);
  for (int i = 0; i < 16 - text.length(); i++)
    lcd.print(" ");
}

// wrapper for restore
void restoreLCDText() {
    updateLCDText(sentenceBuffer);
}

// Legacy wrapper to maintain compatibility with other calls if needed
void updateLCDToneHz(int hz) {
    // If not overlay, update default view
    if (!isOverlayActive) updateDefaultView();
}

void clearLCDText() {
  lcd.clear();
  updateDefaultView();
  lcd.setCursor(0, 1);
  lcd.print("====CLEARED====");
  delay(300); 
  lcd.clear();
  updateDefaultView();
  updateLCDText(sentenceBuffer);
}

#endif
