# Ultimate CW Keyer

A consolidated and feature-rich Arduino-based CW (Morse Code) Keyer, Decoder, and Encoder. This project provides a comprehensive solution for practicing and transmitting Morse code with visual feedback via an LCD and control via a Command Line Interface (CLI).

## Features

- **Dual Mode Operation**: Serves as both a CW Keyer (sending) and Decoder (receiving/interpreting paddle input).
- **Text-to-Morse**: Send text via the Serial Monitor to have it encoded and played back as Morse code.
- **Real-time Decoding**: Decodes paddle input into text, displaying it on a 16x2 I2C LCD.
- **Command Line Interface (CLI)**: Control keyer settings (WPM, Tone, Mode) directly from the Serial Monitor.
- **Hardware Controls**:
  - Potentiometers for WPM (Speed), Tone Frequency, and Volume/Analog.
  - Buttons for Mode switching and Buffer cleaning.
- **Multiple Display Modes**:
  - `CHAR`: Display decoded characters.
  - `MRS`: Display Morse dot/dash sequences.
  - `BOTH`: Display both character and Morse sequence.

## Hardware Setup

The project is designed for an Arduino board (e.g., Uno, Nano) with the following pin configuration (defined in `PinSettings.h`):

| Component | Pin | Description |
|-----------|-----|-------------|
| **Paddle** | | |
| Dot Paddle | D4 | Input for Dot |
| Dash Paddle | D5 | Input for Dash |
| **Controls** | | |
| WPM Pot | A0 | Control Words Per Minute (speed) |
| Vol Pot | A1 | Volume Control (Analog output scaling) |
| Tone Pot | A2 | Tone Frequency Control |
| Function Buttons | A3 | Analog ladder for multiple buttons |
| Mode Button | D3 | Switch between decoding modes |
| Clean Button | D2 | Clear text/morse buffers |
| **Outputs** | | |
| Buzzer | D9 | Audio output for sidetone |
| Headphone Out | D7 | Output for amplifier |
| Radio Out | D6 | Output for radio interface |
| Power LED | D10 | Indicates power status (Red) |
| Morse LED | D8 | Indicates keying status (Green) |
| **Display** | | |
| LCD SDA | A4 | I2C Data |
| LCD SCL | A5 | I2C Clock |

*Note: The LCD address is set to `0x27`.*

## Installation

1. **Dependencies**: Ensure you have the `LiquidCrystal_I2C` library installed in your Arduino IDE.
2. **Flash**: Open `Ultimate_CW_Keyer/Ultimate_CW_Keyer.ino` in Arduino IDE and upload it to your board.
3. **Connect**: Open the Serial Monitor at **9600 baud**.

## Usage

### Command Line Interface (CLI)

Type `help` in the Serial Monitor to see available commands.

- `wpm <5-50>`: Set the Words Per Minute speed.
- `tone <400-1200>`: Set the side-tone frequency in Hz.
- `mode <0|1|2>`: Set the display mode (0=CHAR, 1=MRS, 2=BOTH).
- `silent <0|1>`: Enable/Disable silent mode.
- `clear`: Clear the LCD and internal buffers.
- `status`: Show current WPM, Tone, Mode, and Buffer.
- **Text Input**: Type any sentence to hear it played back in Morse code.

### Paddle Operation

- Use the paddle to enter Morse code.
- The LCD will display the decoded text or dots/dashes based on the selected mode.
- Use the **Mode Button** to cycle through display modes.
- Use the **Clean Button** to reset the display.

## Project Structure

- `Ultimate_CW_Keyer.ino`: Main application logic and loop.
- `PinSettings.h`: Pin definitions and hardware configuration.
- `CLI_Controller.h`: Handles Serial commands and text inputs.
- `Decoder.h`: Logic for converting Morse sequences to characters.
- `Encoder.h`: Logic for converting text to Morse sequences.
- `LCD_Controller.h`: Manages specific LCD updates.
- `Buzzer_LED_Controller.h`: Low-level control for audio and visuals.
- `Potentiometer_Controller.h`: Reads analog inputs for settings.
- `Button_Controller.h`: Debouncing and handling button presses.