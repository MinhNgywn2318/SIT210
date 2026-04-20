import tkinter as tk
from tkinter import ttk
import RPi.GPIO as GPIO

# =========================
# GPIO setup
# =========================
GPIO.setmode(GPIO.BCM)

LIVING_LED = 17
BATHROOM_LED = 27
CLOSET_LED = 22

GPIO.setup(LIVING_LED, GPIO.OUT)
GPIO.setup(BATHROOM_LED, GPIO.OUT)
GPIO.setup(CLOSET_LED, GPIO.OUT)

GPIO.output(LIVING_LED, GPIO.LOW)
GPIO.output(BATHROOM_LED, GPIO.LOW)
GPIO.output(CLOSET_LED, GPIO.LOW)

# =========================
# Functions
# =========================
def turn_off_all():
    GPIO.output(LIVING_LED, GPIO.LOW)
    GPIO.output(BATHROOM_LED, GPIO.LOW)
    GPIO.output(CLOSET_LED, GPIO.LOW)

def select_room():
    room = selected_room.get()
    turn_off_all()

    if room == "Living Room":
        GPIO.output(LIVING_LED, GPIO.HIGH)
        status_label.config(text="Living Room light is ON")
    elif room == "Bathroom":
        GPIO.output(BATHROOM_LED, GPIO.HIGH)
        status_label.config(text="Bathroom light is ON")
    elif room == "Closet":
        GPIO.output(CLOSET_LED, GPIO.HIGH)
        status_label.config(text="Closet light is ON")

def close_program():
    turn_off_all()
    GPIO.cleanup()
    root.destroy()

# =========================
# GUI setup
# =========================
root = tk.Tk()
root.title("Smart Home Light Control")
root.geometry("380x280")
root.resizable(False, False)

title_label = ttk.Label(root, text="Select a Room Light", font=("Arial", 16))
title_label.pack(pady=15)

selected_room = tk.StringVar(value="")

living_radio = ttk.Radiobutton(
    root,
    text="Living Room",
    variable=selected_room,
    value="Living Room",
    command=select_room
)
living_radio.pack(pady=5)

bathroom_radio = ttk.Radiobutton(
    root,
    text="Bathroom",
    variable=selected_room,
    value="Bathroom",
    command=select_room
)
bathroom_radio.pack(pady=5)

closet_radio = ttk.Radiobutton(
    root,
    text="Closet",
    variable=selected_room,
    value="Closet",
    command=select_room
)
closet_radio.pack(pady=5)

status_label = ttk.Label(root, text="No room selected")
status_label.pack(pady=15)

exit_button = ttk.Button(root, text="Exit", command=close_program)
exit_button.pack(pady=15)

root.protocol("WM_DELETE_WINDOW", close_program)

root.mainloop(
