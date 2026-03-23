#include <SPI.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

const char* ssid = "My Linh";
const char* pass = "Phamluan123";

unsigned long channelNumber = 3309137;
const char* writeAPIKey = "3FRXZ1WBW7UJRW8B";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 30000;   // 30 seconds

void connectToWiFi();
void uploadTemperatureToThingSpeak(float temperature);

void setup() {
  Serial.begin(9600);
  delay(2000);

  dht.begin();
  connectToWiFi();
  ThingSpeak.begin(client);

  Serial.println("System started.");
}

void loop() {
  if (millis() - lastUploadTime >= uploadInterval) {
    float temperature = dht.readTemperature();

    if (isnan(temperature)) {
      Serial.println("Failed to read temperature from DHT11 sensor");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");

    uploadTemperatureToThingSpeak(temperature);
    lastUploadTime = millis();
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

void uploadTemperatureToThingSpeak(float temperature) {
  ThingSpeak.setField(1, temperature);

  int response = ThingSpeak.writeFields(channelNumber, writeAPIKey);

  if (response == 200) {
    Serial.println("Temperature uploaded successfully.");
  } else {
    Serial.print("Upload failed. HTTP error code: ");
    Serial.println(response);
  }
}
