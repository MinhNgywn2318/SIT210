# Task 1.1P - Switching ON Lights

## Description
This project is a smart elderly care home lighting prototype using modular programming. The system uses one push button and two LEDs to simulate the porch light and hallway light of Linda’s home.

When the push button is pressed:
- the porch light turns ON for 30 seconds
- the hallway light turns ON for 60 seconds

## Hardware Used
- Arduino board
- 2 LEDs
- 2 resistors
- 1 push button
- Breadboard
- Jumper wires

## Pin Connections
- Button: D2
- Porch LED: D5
- Hallway LED: D6

## Program Structure
The program is written using a modular programming approach.

### setupPins()
This function configures the input and output pins and sets both LEDs to OFF initially.

### handleButtonPress()
This function reads the button state and detects when the user presses the button.

### turnOnLights()
This function turns both lights ON and stores the start time for each light.

### updateLights()
This function checks the elapsed time using millis() and turns each light OFF after its required duration.

## Why Modular Programming Was Used
Modular programming improves readability, reusability, and maintainability. Each function performs one clear task, making the program easier to understand and expand in future tasks.

## Author
Anh Minh Nguyen
