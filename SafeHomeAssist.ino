#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

// -- CHANGE THESE ---------------------------------------------
const char* WIFI_SSID     = "AnhMinh";
const char* WIFI_PASSWORD = "Anhminh18";
const char* MQTT_BROKER   = "192.168.4.27";   // Raspberry Pi IP
// -------------------------------------------------------------

const int   MQTT_PORT     = 1883;
const char* MQTT_CLIENT   = "arduino_safehome";
const char* TOPIC_SENSOR  = "safehome/sensor";
const char* TOPIC_ALERT   = "safehome/alert";

const float LUX_THRESHOLD = 50.0;
const int   LOOP_DELAY    = 2000;
const int   MAX_RETRIES   = 3;
const int   WIFI_TIMEOUT  = 10000;

#define PIR_PIN  2
#define LED_PIN  9

BH1750       lightMeter;
WiFiClient   wifiClient;
PubSubClient mqtt(wifiClient);

// -- Blink LED for error signalling ---------------------------
void blinkError(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_PIN, HIGH); delay(100);
    digitalWrite(LED_PIN, LOW);  delay(100);
  }
}

// -- Connect WiFi (non-blocking with timeout) -----------------
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.print("[WiFi] Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > WIFI_TIMEOUT) {
      Serial.println("\n[WiFi] Timeout.");
      return;
    }
    delay(500); Serial.print(".");
  }
  Serial.println("\n[WiFi] Connected.");
}

// -- Connect MQTT ---------------------------------------------
void connectMQTT() {
  if (mqtt.connected()) return;
  if (WiFi.status() != WL_CONNECTED) return;
  Serial.print("[MQTT] Connecting...");
  if (mqtt.connect(MQTT_CLIENT)) {
    Serial.println(" OK");
  } else {
    Serial.print(" FAIL rc="); Serial.println(mqtt.state());
  }
}

// -- Read BH1750 with retry + fail-safe -----------------------
float readLux() {
  for (int i = 1; i <= MAX_RETRIES; i++) {
    float v = lightMeter.readLightLevel();
    if (v >= 0) return v;
    Serial.print("[Sensor] BH1750 retry "); Serial.println(i);
    delay(500);
  }
  Serial.println("[Sensor] BH1750 fault -- defaulting to low light.");
  blinkError(5);
  return -1.0;
}

// -- SETUP ----------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);
  Serial.println("\n============================");
  Serial.println("  SafeHome Assist -- Startup");
  Serial.println("============================");

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.begin();
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("[Setup] ERROR: BH1750 not found.");
    Serial.println("        Check SDA(A4) SCL(A5) and VCC 3.3V.");
    blinkError(10);
  } else {
    Serial.println("[Setup] BH1750 OK.");
  }

  connectWiFi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  connectMQTT();

  Serial.println("[Setup] Ready.\n");
}

// -- LOOP -----------------------------------------------------
void loop() {
  connectWiFi();
  connectMQTT();
  mqtt.loop();

  bool  motion   = (digitalRead(PIR_PIN) == HIGH);
  float lux      = readLux();
  bool  lowLight = (lux < 0) ? true : (lux < LUX_THRESHOLD);

  // Print to Serial Monitor
  Serial.print("[Loop] Lux: ");
  if (lux < 0) Serial.print("ERR");
  else         Serial.print(lux, 1);
  Serial.print(" | Motion: ");   Serial.print(motion   ? "YES" : "NO ");
  Serial.print(" | LowLight: "); Serial.println(lowLight ? "YES" : "NO");

  // Publish sensor data to Pi
  String sensorData =
    String("{\"lux\":") + (lux < 0 ? "null" : String(lux, 1)) +
    ",\"motion\":"      + (motion   ? "true" : "false") +
    ",\"lowLight\":"    + (lowLight ? "true" : "false") + "}";
  mqtt.publish(TOPIC_SENSOR, sensorData.c_str());

  // Alert condition: motion AND low light
  if (motion && lowLight) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("[Alert] TRIGGERED -- motion detected in low light!");
    String alertData =
      String("{\"event\":\"motion_low_light\"") +
      ",\"lux\":"  + (lux < 0 ? "null" : String(lux, 1)) +
      ",\"motion\":true}";
    mqtt.publish(TOPIC_ALERT, alertData.c_str());
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(LOOP_DELAY);
}
