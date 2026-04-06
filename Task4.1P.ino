const int PIR_PIN = 6;
const int LED1_PIN = 2;
const int LED2_PIN = 3;

volatile bool motionDetected = false;

void pirISR() {
  motionDetected = true;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);

  Serial.println("System ready.");
  Serial.println("Waiting for motion...");
}

void loop() {
  if (motionDetected) {
    motionDetected = false;

    Serial.println("Motion detected by PIR interrupt.");
    Serial.println("LED1 and LED2 turned ON.");

    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);

    delay(5000);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    Serial.println("LED1 and LED2 turned OFF.");
    Serial.println("Waiting for motion...");
  }
}
