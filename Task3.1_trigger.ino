#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

char ssid[] = "AnhMinh";
char pass[] = "Anhminh18";

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;

const char* topicLux = "deakin/minh/terrarium/lux";
const char* topicStatus = "deakin/minh/terrarium/status";

BH1750 lightMeter;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

const float sunlightThreshold = 15.0;   // threshold thấp để dễ test
bool sunlightDetected = false;
bool previousSunlightDetected = false;

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 5000;

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  while (!client.connected()) {
    String clientId = "Nano33IoTClient-";
    clientId += String(random(1000, 9999));

    Serial.println("Connecting to MQTT...");
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("MQTT failed, state = ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  Wire.begin();

  if (lightMeter.begin()) {
    Serial.println("BH1750 started successfully");
  } else {
    Serial.println("Error initialising BH1750");
  }

  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  connectToMQTT();

  Serial.println("System started.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  if (!client.connected()) {
    connectToMQTT();
  }

  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - lastPublishTime >= publishInterval) {
    lastPublishTime = currentMillis;

    float lux = lightMeter.readLightLevel();

    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lux");

    String luxString = String(lux, 2);
    client.publish(topicLux, luxString.c_str());

    sunlightDetected = (lux >= sunlightThreshold);

    if (sunlightDetected != previousSunlightDetected) {
      if (sunlightDetected) {
        client.publish(topicStatus, "SUNLIGHT_ON");
        Serial.println("Notification Trigger: Sunlight started");
      } else {
        client.publish(topicStatus, "SUNLIGHT_OFF");
        Serial.println("Notification Trigger: Sunlight stopped");
      }

      previousSunlightDetected = sunlightDetected;
    }
  }
}
