const int pirPin = 2;
const int switchPin = 3;
const int led1Pin = 5;
const int led2Pin = 6;

volatile bool pirEvent = false;
volatile bool switchEvent = false;
bool lightsOn = false;

void pirISR() {
  pirEvent = true;
}

void switchISR() {
  switchEvent = true;
}

void setup() {
  Serial.begin(9600);
  delay(2000);

  pinMode(pirPin, INPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  attachInterrupt(digitalPinToInterrupt(pirPin), pirISR, RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), switchISR, FALLING);

  Serial.println("System ready.");
}

void loop() {
  if (pirEvent) {
    pirEvent = false;
    lightsOn = !lightsOn;
    digitalWrite(led1Pin, lightsOn);
    digitalWrite(led2Pin, lightsOn);
    Serial.println("PIR interrupt detected.");
    delay(1000);
  }

  if (switchEvent) {
    switchEvent = false;
    lightsOn = !lightsOn;
    digitalWrite(led1Pin, lightsOn);
    digitalWrite(led2Pin, lightsOn);
    Serial.println("Switch interrupt detected.");
    delay(300);
  }
}
