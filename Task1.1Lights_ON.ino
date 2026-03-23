// Task 1.1P - Arduino Uno version

const int BUTTON_PIN = 2;
const int PORCH_LED_PIN = 5;
const int HALLWAY_LED_PIN = 6;

const unsigned long PORCH_DURATION = 30000;    // 30 seconds
const unsigned long HALLWAY_DURATION = 60000;  // 60 seconds

bool porchLightOn = false;
bool hallwayLightOn = false;

unsigned long porchStartTime = 0;
unsigned long hallwayStartTime = 0;

bool lastButtonState = HIGH;

void setupPins();
void handleButtonPress();
void turnOnLights();
void updateLights();

void setup() {
  Serial.begin(9600);
  setupPins();
  Serial.println("System started");
}

void loop() {
  handleButtonPress();
  updateLights();
}

void setupPins() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(PORCH_LED_PIN, OUTPUT);
  pinMode(HALLWAY_LED_PIN, OUTPUT);

  digitalWrite(PORCH_LED_PIN, LOW);
  digitalWrite(HALLWAY_LED_PIN, LOW);
}

void handleButtonPress() {
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    turnOnLights();
    Serial.println("Button pressed - lights ON");
  }

  lastButtonState = currentButtonState;
}

void turnOnLights() {
  porchLightOn = true;
  hallwayLightOn = true;

  porchStartTime = millis();
  hallwayStartTime = millis();

  digitalWrite(PORCH_LED_PIN, HIGH);
  digitalWrite(HALLWAY_LED_PIN, HIGH);
}

void updateLights() {
  unsigned long currentTime = millis();

  if (porchLightOn && currentTime - porchStartTime >= PORCH_DURATION) {
    porchLightOn = false;
    digitalWrite(PORCH_LED_PIN, LOW);
    Serial.println("Porch light OFF");
  }

  if (hallwayLightOn && currentTime - hallwayStartTime >= HALLWAY_DURATION) {
    hallwayLightOn = false;
    digitalWrite(HALLWAY_LED_PIN, LOW);
    Serial.println("Hallway light OFF");
  }
}
