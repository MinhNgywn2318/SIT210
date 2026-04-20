import tkinter as tk
import RPi.GPIO as GPIO

# ===== GPIO Setup =====
LED_PIN = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

pwm = GPIO.PWM(LED_PIN, 1000)  # 1000 Hz
pwm.start(0)  # start with LED off

# ===== Functions =====
def turn_on():
    brightness = slider.get()
    pwm.ChangeDutyCycle(brightness)
    status_label.config(text=f"Light ON - Brightness: {brightness}%")

def turn_off():
    pwm.ChangeDutyCycle(0)
    status_label.config(text="Light OFF")

def update_brightness(value):
    brightness = int(float(value))
    pwm.ChangeDutyCycle(brightness)
    status_label.config(text=f"Brightness: {brightness}%")

def close_program():
    pwm.stop()
    GPIO.cleanup()
    window.destroy()

# ===== GUI Setup =====
window = tk.Tk()
window.title("Living Room Light Control")
window.geometry("400x300")

title_label = tk.Label(window, text="Living Room Light Control", font=("Arial", 16))
title_label.pack(pady=10)

slider = tk.Scale(window, from_=0, to=100, orient=tk.HORIZONTAL,
                  length=250, label="Brightness (%)", command=update_brightness)
slider.pack(pady=20)
slider.set(50)

on_button = tk.Button(window, text="ON", width=12, height=2, command=turn_on)
on_button.pack(pady=5)

off_button = tk.Button(window, text="OFF", width=12, height=2, command=turn_off)
off_button.pack(pady=5)

status_label = tk.Label(window, text="System Ready", font=("Arial", 12))
status_label.pack(pady=15)

window.protocol("WM_DELETE_WINDOW", close_program)
window.mainloop()
