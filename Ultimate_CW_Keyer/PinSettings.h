#ifndef PINSETTINGS_H
#define PINSETTINGS_H

// ----------------------------------------------------------------------------
// PADDLE INPUT
// ----------------------------------------------------------------------------
#define dotPin 4
#define dashPin 5

// ----------------------------------------------------------------------------
// POTENTIOMETERS
// ----------------------------------------------------------------------------
#define WPMPotPin A0  // Controlling WPM
#define VOLPotPin A1  // "VOLPot" can control vol only output for "BUZZER" and "RADIO OUT"
#define TONEPotPin A2 // "TONEPot" can control the tone only for "BUZZER" and "RADIO OUT"

// ----------------------------------------------------------------------------
// SOUND OUTPUT
// ----------------------------------------------------------------------------
#define BUZZERPin 9
#define HeadphoneOutPin 7 // "HeadphoneOutPin" send signal to amplifier (powered By LM386)
#define RadioOutPin 6     // "RadioOutPin" send signal to IN-LINE to MIC-LINE converter (RRC)

// ----------------------------------------------------------------------------
// LEDS
// ----------------------------------------------------------------------------
#define PWRLED 10  // "PWRLED" is the RED LED.
#define MRSLED 8   // "MRSLED" is the GREEN LED.

// ----------------------------------------------------------------------------
// BUTTONS
// ----------------------------------------------------------------------------
#define MODEButton 3       // Changing mode between Morse-Char-Mors&Char
#define CLEANButton 2      // Cleaning the buffer
#define FunctionButtons A3 // 5 different programmable buttons

// ----------------------------------------------------------------------------
// SCREEN   [ I2C ]
// ----------------------------------------------------------------------------
// A4 (SDA), A5 (SCL)
#define I2C_ADDR 0x27

#endif
