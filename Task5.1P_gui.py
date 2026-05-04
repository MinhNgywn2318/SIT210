import tkinter as tk
from tkinter import ttk
import RPi.GPIO as GPIO

# =========================
# GPIO setup
# =========================
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

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
def update_lights():
    if living_var.get():
        GPIO.output(LIVING_LED, GPIO.HIGH)
    else:
        GPIO.output(LIVING_LED, GPIO.LOW)

    if bathroom_var.get():
        GPIO.output(BATHROOM_LED, GPIO.HIGH)
    else:
        GPIO.output(BATHROOM_LED, GPIO.LOW)

    if closet_var.get():
        GPIO.output(CLOSET_LED, GPIO.HIGH)
    else:
        GPIO.output(CLOSET_LED, GPIO.LOW)

    selected = []
    if living_var.get():
        selected.append("Living Room")
    if bathroom_var.get():
        selected.append("Bathroom")
    if closet_var.get():
        selected.append("Closet")

    if selected:
        status_label.config(text="ON: " + ", ".join(selected))
    else:
        status_label.config(text="All lights are OFF")


def turn_off_all():
    living_var.set(False)
    bathroom_var.set(False)
    closet_var.set(False)

    GPIO.output(LIVING_LED, GPIO.LOW)
    GPIO.output(BATHROOM_LED, GPIO.LOW)
    GPIO.output(CLOSET_LED, GPIO.LOW)

    status_label.config(text="All lights are OFF")


def turn_on_all():
    living_var.set(True)
    bathroom_var.set(True)
    closet_var.set(True)

    GPIO.output(LIVING_LED, GPIO.HIGH)
    GPIO.output(BATHROOM_LED, GPIO.HIGH)
    GPIO.output(CLOSET_LED, GPIO.HIGH)

    status_label.config(text="All lights are ON")


def close_program():
    turn_off_all()
    GPIO.cleanup()
    root.destroy()


# =========================
# GUI setup
# =========================
root = tk.Tk()
root.title("Smart Home Light Control")
root.geometry("400x350")
root.resizable(False, False)

title_label = ttk.Label(root, text="Select Room Lights", font=("Arial", 16))
title_label.pack(pady=15)

living_var = tk.BooleanVar(value=False)
bathroom_var = tk.BooleanVar(value=False)
closet_var = tk.BooleanVar(value=False)

living_check = ttk.Checkbutton(
    root,
    text="Living Room",
    variable=living_var,
    command=update_lights
)
living_check.pack(pady=5)

bathroom_check = ttk.Checkbutton(
    root,
    text="Bathroom",
    variable=bathroom_var,
    command=update_lights
)
bathroom_check.pack(pady=5)

closet_check = ttk.Checkbutton(
    root,
    text="Closet",
    variable=closet_var,
    command=update_lights
)
closet_check.pack(pady=5)

status_label = ttk.Label(root, text="All lights are OFF")
status_label.pack(pady=15)

all_on_button = ttk.Button(root, text="Turn All ON", command=turn_on_all)
all_on_button.pack(pady=5)

all_off_button = ttk.Button(root, text="Turn All OFF", command=turn_off_all)
all_off_button.pack(pady=5)

exit_button = ttk.Button(root, text="Exit", command=close_program)
exit_button.pack(pady=15)

root.protocol("WM_DELETE_WINDOW", close_program)

root.mainloop()
