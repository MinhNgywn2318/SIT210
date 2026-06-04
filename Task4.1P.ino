#include <Wire.h>
#include <BH1750.h>

const int pirPin    = 2;
const int switchPin = 3;
const int led1Pin   = 5;
const int led2Pin   = 6;

const float DARK_THRESHOLD = 50.0;

BH1750 lightMeter;

volatile bool pirEvent    = false;
volatile bool switchEvent = false;

bool lightsOn = false;

volatile unsigned long lastPirTime    = 0;
volatile unsigned long lastSwitchTime = 0;
const unsigned long PIR_DEBOUNCE    = 1000;
const unsigned long SWITCH_DEBOUNCE = 300;

void pirISR() {
  unsigned long now = millis();
  if (now - lastPirTime > PIR_DEBOUNCE) {
    pirEvent = true;
    lastPirTime = now;
  }
}

void switchISR() {
  unsigned long now = millis();
  if (now - lastSwitchTime > SWITCH_DEBOUNCE) {
    switchEvent = true;
    lastSwitchTime = now;
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  delay(2000);

  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 initialised successfully.");
  } else {
    Serial.println("ERROR: BH1750 not found. Check wiring.");
  }

  pinMode(pirPin,    INPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(led1Pin,   OUTPUT);
  pinMode(led2Pin,   OUTPUT);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  attachInterrupt(digitalPinToInterrupt(pirPin),    pirISR,    RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), switchISR, FALLING);

  Serial.println("System ready.");
  Serial.println("PIR interrupt enabled on D2.");
  Serial.println("Switch interrupt enabled on D3.");
  Serial.println("─────────────────────────────────");
}

void loop() {
  float lux = lightMeter.readLightLevel();
  bool isDark = (lux < DARK_THRESHOLD);

  if (pirEvent) {
    pirEvent = false;
    Serial.print("[PIR]    Motion detected | Light: ");
    Serial.print(lux);
    Serial.print(" lx | Condition: ");

    if (isDark) {
      lightsOn = true;
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin, HIGH);
      Serial.println("DARK -> Lights ON");
    } else {
      Serial.println("BRIGHT -> Lights unchanged");
    }
  }

  if (switchEvent) {
    switchEvent = false;
    lightsOn = !lightsOn;
    digitalWrite(led1Pin, lightsOn ? HIGH : LOW);
    digitalWrite(led2Pin, lightsOn ? HIGH : LOW);
    Serial.print("[SWITCH] Manual override | Light: ");
    Serial.print(lux);
    Serial.print(" lx | Lights ");
    Serial.println(lightsOn ? "ON" : "OFF");
  }

  delay(200);
}
