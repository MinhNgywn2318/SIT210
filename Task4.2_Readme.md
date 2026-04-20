# Task 4.2D – Calling a Function from the Web

## Student
Anh Minh Nguyen

## Overview
This project demonstrates how an Arduino Nano 33 IoT can host a web page that allows a user to remotely control three LEDs representing lights in Linda’s home: living room, bathroom, and closet.

## Hardware
- Arduino Nano 33 IoT
- Breadboard
- 3 LEDs
- 3 resistors
- Jumper wires

## Software
- Arduino IDE / Arduino Web Editor
- WiFiNINA library

## System Description
The Arduino connects to Wi-Fi and runs a simple web server. A browser opens the web page hosted by the Arduino. When the user clicks one of the room buttons, the browser sends an HTTP request to the Arduino. The Arduino then calls the `toggleLight()` function with a string parameter and toggles the corresponding LED.

## Files
- `Task4.2DCloudFunction.ino` – Arduino code
- `images/` – hardware and software screenshots

## Function Used
- `toggleLight("living room")`
- `toggleLight("bathroom")`
- `toggleLight("closet")`



