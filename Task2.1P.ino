#include <SPI.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define LIGHTPIN A0

const char* ssid = "AnhMinh";
const char* pass = "Anhminh18";

unsigned long channelNumber = 3319081;
const char* writeAPIKey = "FYMC6JCP6YVOMK38";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 5000;   // 5 seconds for testing

void connectToWiFi();
float readTemperature();
int readLightLevel();
void uploadDataToThingSpeak(float temperature, int lightLevel);

void setup() {
  Serial.begin(9600);
  delay(2000);

  dht.begin();
  delay(2000);

  connectToWiFi();
  ThingSpeak.begin(client);

  Serial.println("System started.");
}

void loop() {
  if (millis() - lastUploadTime >= uploadInterval) {
    Serial.println("Reading sensors...");

    float temperature = readTemperature();
    int lightLevel = readLightLevel();

    if (isnan(temperature)) {
      Serial.println("Failed to read temperature from DHT11 sensor");
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" *C");
    }

    Serial.print("Light level: ");
    Serial.println(lightLevel);

    if (!isnan(temperature)) {
      uploadDataToThingSpeak(temperature, lightLevel);
    }

    lastUploadTime = millis();
    Serial.println("----------------------");
  }
}

void connectToWiFi() {
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

float readTemperature() {
  return dht.readTemperature();
}

int readLightLevel() {
  return analogRead(LIGHTPIN);
}

void uploadDataToThingSpeak(float temperature, int lightLevel) {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, lightLevel);

  int response = ThingSpeak.writeFields(channelNumber, writeAPIKey);

  if (response == 200) {
    Serial.println("Temperature and light uploaded successfully.");
  } else {
    Serial.print("Upload failed. HTTP error code: ");
    Serial.println(response);
  }
}
