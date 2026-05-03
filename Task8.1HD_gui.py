import serial
import time

# Arduino is connected to Raspberry Pi at /dev/ttyACM0
arduino = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2)

def convert_text_to_command(text):
    text = text.lower()

    if "bathroom" in text and "on" in text:
        return "BATHROOM_ON"

    elif "hallway" in text and "on" in text:
        return "HALLWAY_ON"

    elif "fan" in text and "on" in text:
        return "FAN_ON"

    elif "off" in text:
        return "ALL_OFF"

    else:
        return "UNKNOWN"

print("Task 8.1HD Text Command Simulation")
print("Type commands such as:")
print("- turn on bathroom light")
print("- turn on hallway light")
print("- turn on fan")
print("- turn off all")
print("Type exit to stop the program")

while True:
    text = input("\nEnter command text: ")

    if text.lower() == "exit":
        print("Program stopped")
        break

    command = convert_text_to_command(text)
    print("Arduino command:", command)

    if command != "UNKNOWN":
        arduino.write((command + "\n").encode())
        print("Command sent to Arduino")
    else:
        print("Command not recognised")
