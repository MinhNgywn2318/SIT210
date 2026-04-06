# Task 3.2C MQTT

## Student Details
- Name: Anh Minh Nguyen
- Unit: SIT210 Embedded Systems Development

## Overview
This project demonstrates MQTT publish-subscribe communication using an Arduino Nano 33 IoT, an HC-SR04 ultrasonic sensor, and two LEDs. The system detects hand gestures and controls two lights representing the bathroom and hallway.

## Hardware Used
- Arduino Nano 33 IoT
- HC-SR04 Ultrasonic Sensor
- 2 LEDs
- 2 Resistors
- Breadboard
- Jumper wires

## MQTT Broker Details
- Broker: broker.emqx.io
- Port: 1883

## Topics Used
- ES/AnhMinh/Wave
- ES/AnhMinh/Pat

## System Logic
- If the measured distance is between 5 cm and 15 cm, the system detects a wave and publishes a message to ES/AnhMinh/Wave.
- If the measured distance is between 0 cm and 5 cm, the system detects a pat and publishes a message to ES/AnhMinh/Pat.
- When the Arduino receives a message on ES/AnhMinh/Wave, both LEDs turn ON.
- When the Arduino receives a message on ES/AnhMinh/Pat, both LEDs turn OFF.

## Pin Connections
### Ultrasonic Sensor
- VCC -> Vin
- GND -> GND
- TRIG -> D6
- ECHO -> D7

### LEDs
- Bathroom LED -> D2
- Hallway LED -> D3

## Files Included
- Source code: `src/Task3.2C.ino`

