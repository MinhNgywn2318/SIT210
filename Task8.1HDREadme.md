# Task 8.1HD

## Student
Anh Minh Nguyen

## Unit
SIT210 / SIT730 Embedded Systems Development

## Task Overview
This project implements a prototype of a voice activated lighting system using a Raspberry Pi and an Arduino Nano 33 IoT. The aim of the system is to help Linda turn on the bathroom light, hallway light, and exhaust fan using simple commands.

In the complete design, the Raspberry Pi receives voice input through a USB microphone and converts the audio into text using speech recognition. During prototype testing, a USB microphone was not available, so text input was used to simulate the recognised speech result. The rest of the system logic remains the same.

## Hardware Used
- Raspberry Pi
- Arduino Nano 33 IoT
- BH1750 light sensor
- LEDs
- Resistors
- Breadboard
- Jumper wires
- USB cable

## System Design
The Raspberry Pi is responsible for command processing. It receives a command sentence such as "turn on bathroom light", converts it into an Arduino command such as `BATHROOM_ON`, and sends it to the Arduino.

The Arduino Nano 33 IoT receives the command, reads the BH1750 light sensor, and controls the output LEDs. The LEDs represent the bathroom light, hallway light, and exhaust fan.

## Commands
| Input Text | Arduino Command | Output |
|---|---|---|
| turn on bathroom light | BATHROOM_ON | Turns on bathroom LED if the light level is low |
| turn on hallway light | HALLWAY_ON | Turns on hallway LED if the light level is low |
| turn on fan | FAN_ON | Turns on fan LED |
| turn off all | ALL_OFF | Turns off all LEDs |

## Data Flow
Voice Command / Text Input Simulation  
→ Raspberry Pi  
→ Command Processing  
→ Serial Communication via USB  
→ Arduino Nano 33 IoT  
→ BH1750 Light Sensor Reading  
→ LED / Fan Control

## Arduino Code Explanation
The Arduino code has three main sections.

First, it initialises the BH1750 light sensor and the LED output pins. The LEDs represent the bathroom light, hallway light, and exhaust fan.

Second, it waits for incoming commands from the Raspberry Pi through serial communication.

Third, when a command is received, the Arduino reads the current light level from the BH1750 sensor. If the command is for the bathroom or hallway light, the Arduino checks whether the light level is below the threshold. If the room is dark, the relevant LED is turned on. If the room is bright enough, the LED remains off. The fan command turns on the fan LED directly, and the ALL_OFF command turns off all outputs.

## Raspberry Pi Code Explanation
The Raspberry Pi Python code accepts text input that simulates the recognised speech output. It converts the input sentence into one of four Arduino commands: `BATHROOM_ON`, `HALLWAY_ON`, `FAN_ON`, or `ALL_OFF`.

After conversion, the command is sent to the Arduino through the serial port `/dev/ttyACM0`.

## Prototype Limitation
A USB microphone was not available during testing. Therefore, text input was used to simulate the recognised speech text. In the complete system, this text would come from the speech recognition module after processing audio from a microphone.

The command processing, Raspberry Pi-to-Arduino communication, BH1750 light sensor reading, and LED/fan output control were successfully implemented and tested.


