#ifndef BUZZER_LED_CONTROLLER_H
#define BUZZER_LED_CONTROLLER_H

#include "PinSettings.h"
#include "GlobalVars.h"

void stopTone() {
  noTone(BUZZERPin);
  digitalWrite(MRSLED, LOW);
  Serial.print("]"); // Real-time OFF token
}

void playDash() {
  Serial.print("["); // Real-time ON token
  if (!silentMode) tone(BUZZERPin, toneHz);
  digitalWrite(MRSLED, HIGH);
}

void playDot() {
  Serial.print("["); // Real-time ON token
  if (!silentMode) tone(BUZZERPin, toneHz);
  digitalWrite(MRSLED, HIGH);
}

#endif
