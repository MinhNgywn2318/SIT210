#include <WiFiNINA.h>
#include <PubSubClient.h>

// ===================== WIFI =====================
char ssid[] = "AnhMinh";
char pass[] = "Anhminh18";

// ===================== MQTT =====================
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;

const char* topicWave = "ES/AnhMinh/Wave";
const char* topicPat  = "ES/AnhMinh/Pat";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// ===================== PINS =====================
const int trigPin = 6;
const int echoPin = 7;

const int ledBathroom = 2;
const int ledHallway = 3;

// ===================== SETTINGS =====================
const char* studentName = "Anh Minh Nguyen";

unsigned long lastDetectTime = 0;
const unsigned long cooldown = 2000;   // 2 seconds

// ===================== WIFI FUNCTION =====================
void connectWiFi() {
  Serial.print("Connecting to WiFi");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ===================== MQTT CALLBACK =====================
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(message);

  if (String(topic) == topicWave) {
    digitalWrite(ledBathroom, HIGH);
    digitalWrite(ledHallway, HIGH);
    Serial.println("Both LEDs turned ON");
  }
  else if (String(topic) == topicPat) {
    digitalWrite(ledBathroom, LOW);
    digitalWrite(ledHallway, LOW);
    Serial.println("Both LEDs turned OFF");
  }
}

// ===================== MQTT RECONNECT =====================
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    String clientId = "Nano33IoT-";
    clientId += String(random(1000, 9999));

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      client.subscribe(topicWave);
      client.subscribe(topicPat);

      Serial.println("Subscribed to custom topics:");
      Serial.println(topicWave);
      Serial.println(topicPat);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 3 seconds");
      delay(3000);
    }
  }
}

// ===================== ULTRASONIC FUNCTION =====================
float readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 50000);

  if (duration == 0) {
    return -1;
  }

  float distance = duration * 0.0343 / 2.0;
  return distance;
}

// ===================== GESTURE DETECTION =====================
void detectGestureAndPublish() {
  float distance = readDistanceCM();

  if (distance < 0) {
    Serial.println("No reading");
    return;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (millis() - lastDetectTime < cooldown) {
    return;
  }

  // Wave: 5 cm to 15 cm
  if (distance > 5 && distance <= 15) {
    Serial.println("Wave detected");
    client.publish(topicWave, studentName);
    lastDetectTime = millis();
  }
  // Pat: 0 cm to 5 cm
  else if (distance > 0 && distance <= 5) {
    Serial.println("Pat detected");
    client.publish(topicPat, studentName);
    lastDetectTime = millis();
  }
}

// ===================== SETUP =====================
void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledBathroom, OUTPUT);
  pinMode(ledHallway, OUTPUT);

  digitalWrite(ledBathroom, LOW);
  digitalWrite(ledHallway, LOW);

  connectWiFi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

// ===================== LOOP =====================
void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();
  detectGestureAndPublish();

  delay(300);
}
