/*
  Ultimate CW Keyer
  Consolidated and Fixed Version
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "PinSettings.h"
#include "GlobalVars.h"

// Define Globals
EncodeState encodeState = IDLE_ENCODE;
String encodeBuffer = "";
int charIndex = 0;
int symbolIndex = 0;
unsigned long encodeStartTime = 0;
String currentMorse = "";

int WPM = 20;
int dotDuration = 1200 / 20;
int lastWPM = -1;

String morseBuffer = "";
String sentenceBuffer = "";
bool lastWasDot = false;
bool justDecoded = false;
int decoderMode = 0; // 0=CHAR, 1=MRS, 2=BOTH
int keyingLength = 0;

int toneHz = 700;
int lastToneHz = -1;
const int minHz = 400;
const int maxHz = 1200;
bool silentMode = false;

// Morse Table Definition
const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",         // S-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....",    // 0-5
  "--...", "---..", "----."                                        // 6-9
};

// LCD Instance
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);


// Include Controllers (AFTER globals defined or declared)
#include "LCD_Controller.h"
#include "Buzzer_LED_Controller.h"
#include "Potentiometer_Controller.h"
#include "Button_Controller.h"
#include "Encoder.h"
#include "Decoder.h"
#include "CLI_Controller.h" // Added CLI

// Keying State
enum KeyingState { IDLE_KEY, SENDING_DOT, SENDING_DASH, POST_DELAY };
KeyingState keyingState = IDLE_KEY;
unsigned long keyingStartTime = 0;
char queuedElement = '\0';
unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 0;

void setup() {
    // Serial.begin(115200);
    Serial.begin(9600);
    // Wait for serial to stabilize (optional)
    delay(100);
    // Serial.println(F("Ultimate CW Keyer Ready. Type 'help' for commands."));
    Serial.println(F("\n"));
    Serial.println(F("\t--- CW Keyer CLI ---"));
    Serial.println(F("\tType 'help' for commands."));
    
    // Pin Modes
    pinMode(dotPin, INPUT_PULLUP);
    pinMode(dashPin, INPUT_PULLUP);
    pinMode(BUZZERPin, OUTPUT);
    pinMode(MRSLED, OUTPUT);
    pinMode(PWRLED, OUTPUT);
    pinMode(MODEButton, INPUT_PULLUP);
    pinMode(CLEANButton, INPUT_PULLUP);
    pinMode(FunctionButtons, INPUT_PULLUP); // Enable Pullup for Analog Ladder
    
    digitalWrite(PWRLED, HIGH);

    lcdInit();
    
    // Initial display
    updateLCDToneHz(toneHz); 
}

void startKeying(unsigned long now) {
  keyingState = (queuedElement == '.') ? SENDING_DOT : SENDING_DASH;
  keyingStartTime = now;
  lastKeyTime = now;
  justDecoded = false;
}

void loop() {
    unsigned long now = millis();
    
    // 0. Serial / CLI Input
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        // If not a system command, send as morse
        if (!processCLI(input)) {
             startEncoding(input);
        }
    }

    // 1. Inputs
    checkPotentiometers();
    checkButtons();
    lcdService(); // Check for overlay timeout
    
    // 2. Paddle Logic
    bool dotPressed = digitalRead(dotPin) == LOW;
    bool dashPressed = digitalRead(dashPin) == LOW;

    if (keyingState == IDLE_KEY) {
        if (dotPressed && !dashPressed) {
          queuedElement = '.';
          keyingLength = dotDuration;
          playDot();
          morseBuffer += ".";
          startKeying(now);
        } else if (dashPressed && !dotPressed) {
          queuedElement = '-';
          keyingLength = dotDuration * 3;
          playDash();
          morseBuffer += "-";
          startKeying(now);
        } else if (dotPressed && dashPressed) {
          queuedElement = lastWasDot ? '-' : '.';
          keyingLength = (queuedElement == '.') ? dotDuration : dotDuration * 3;
          if (queuedElement == '.') playDot(); else playDash();
          morseBuffer += queuedElement;
          startKeying(now);
          lastWasDot = !lastWasDot;
        }
    } else if (keyingState == SENDING_DOT || keyingState == SENDING_DASH) {
        if (now - keyingStartTime >= keyingLength) {
          stopTone();
          keyingStartTime = now;
          keyingState = POST_DELAY;
          keyingLength = dotDuration; // Inter-element gap
        }
    } else if (keyingState == POST_DELAY && now - keyingStartTime >= keyingLength) {
        keyingState = IDLE_KEY;
    }

    // 3. Decode Logic
    // If silence for 3 units, decode char
  if (morseBuffer.length() > 0 && !dotPressed && !dashPressed &&
      now - lastKeyTime >= dotDuration * 3 && !justDecoded) {
    decodeMorse(morseBuffer);
    morseBuffer = "";
    justDecoded = true;
    lastDecodeTime = now;
    // Serial.println(sentenceBuffer);
    updateLCDText(sentenceBuffer);
  }

  // 4. Word Gap logic
  // If silence for 7 units, add space
  if (justDecoded && !dotPressed && !dashPressed &&
      now - lastDecodeTime >= dotDuration * 7) {
    if (!sentenceBuffer.endsWith(" ")) {
        sentenceBuffer += " ";
        Serial.print(" ");
        updateLCDText(sentenceBuffer);
    }
    justDecoded = false;
  }

    // 5. Encoder (Serial to Morse)
    handleEncoding(now);
}
