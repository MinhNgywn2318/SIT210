#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

// LED pins
const int bathroomLed = 2;
const int hallwayLed = 3;
const int fanLed = 4;

// If lux is lower than this value, the room is considered dark
float lightThreshold = 50.0;

float readLux() {
  lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);
  delay(200);

  float lux = lightMeter.readLightLevel();

  if (lux < 0) {
    Serial.println("BH1750 read error");
  }

  return lux;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();
  delay(300);

  pinMode(bathroomLed, OUTPUT);
  pinMode(hallwayLed, OUTPUT);
  pinMode(fanLed, OUTPUT);

  digitalWrite(bathroomLed, LOW);
  digitalWrite(hallwayLed, LOW);
  digitalWrite(fanLed, LOW);

  bool status = lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE, 0x23);

  if (status) {
    Serial.println("BH1750 configured successfully at 0x23");
  } else {
    Serial.println("BH1750 configuration failed");
  }

  delay(500);

  Serial.println("Arduino Voice Activated Lighting System Ready");
  Serial.println("Commands:");
  Serial.println("BATHROOM_ON");
  Serial.println("HALLWAY_ON");
  Serial.println("FAN_ON");
  Serial.println("ALL_OFF");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    float lux = readLux();

    if (lux < 0) {
      Serial.println("Cannot process command because light sensor reading failed");
      return;
    }

    Serial.print("Command received: ");
    Serial.println(command);

    Serial.print("Light level: ");
    Serial.print(lux);
    Serial.println(" lx");

    if (command == "BATHROOM_ON") {
      if (lux < lightThreshold) {
        digitalWrite(bathroomLed, HIGH);
        Serial.println("Bathroom light ON");
      } else {
        digitalWrite(bathroomLed, LOW);
        Serial.println("Bathroom is bright enough");
      }
    }

    else if (command == "HALLWAY_ON") {
      if (lux < lightThreshold) {
        digitalWrite(hallwayLed, HIGH);
        Serial.println("Hallway light ON");
      } else {
        digitalWrite(hallwayLed, LOW);
        Serial.println("Hallway is bright enough");
      }
    }

    else if (command == "FAN_ON") {
      digitalWrite(fanLed, HIGH);
      Serial.println("Exhaust fan ON");
    }

    else if (command == "ALL_OFF") {
      digitalWrite(bathroomLed, LOW);
      digitalWrite(hallwayLed, LOW);
      digitalWrite(fanLed, LOW);
      Serial.println("All devices OFF");
    }

    else {
      Serial.println("Unknown command");
    }
  }
}
