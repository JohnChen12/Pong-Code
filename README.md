# Arduino Pong Game

This is a simple 2-player recreation of the classic arcade game, PONG. It is written for Arduino, using a TFT screen and button controls.

## Hardware Requirements

- Arduino Uno R3(or compatible)
- 2 x pushbuttons per player (4 total)
- ILI9341 TFT display (SPI-based)
- Jumper wires, breadboard, etc.
- 14 resistors (4 - 1000 ohm) (5 - 3.3k ohm) (5 - 2.2k ohm)

## Pin Setup

### Display:
- CS: Pin 8  
- DC: Pin 10  
- RST: Pin 9  
(SPI uses default MOSI, MISO, SCK)

### Buttons:
- Player 1 UP: Pin 3  
- Player 1 DOWN: Pin 4  
- Player 2 UP: Pin 5  
- Player 2 DOWN: Pin 6  

## Features

- Real-time paddle and ball movement
- Scorekeeping
- Victory screen when score reaches 9
- Restart after game over

## Getting Started

1. Install the required libraries:
    - `Adafruit_GFX`
    - `Adafruit_ILI9341`
2. Upload the `.ino` file to your Arduino.
3. Wire the display and buttons according to the pin setup.
