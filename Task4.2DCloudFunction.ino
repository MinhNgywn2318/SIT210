#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "AnhMinh";
char pass[] = "Anhminh18";

WiFiServer server(80);

// LED pins
const int livingRoomLED = 2;
const int bathroomLED   = 3;
const int closetLED     = 4;

// LED states
bool livingState = false;
bool bathroomState = false;
bool closetState = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(livingRoomLED, OUTPUT);
  pinMode(bathroomLED, OUTPUT);
  pinMode(closetLED, OUTPUT);

  digitalWrite(livingRoomLED, LOW);
  digitalWrite(bathroomLED, LOW);
  digitalWrite(closetLED, LOW);

  Serial.print("Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("Arduino IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Web server started.");
}

// Function required by task
void toggleLight(String room) {
  room.toLowerCase();

  if (room == "living room") {
    livingState = !livingState;
    digitalWrite(livingRoomLED, livingState ? HIGH : LOW);
    Serial.print("Living room LED: ");
    Serial.println(livingState ? "ON" : "OFF");
  }
  else if (room == "bathroom") {
    bathroomState = !bathroomState;
    digitalWrite(bathroomLED, bathroomState ? HIGH : LOW);
    Serial.print("Bathroom LED: ");
    Serial.println(bathroomState ? "ON" : "OFF");
  }
  else if (room == "closet") {
    closetState = !closetState;
    digitalWrite(closetLED, closetState ? HIGH : LOW);
    Serial.print("Closet LED: ");
    Serial.println(closetState ? "ON" : "OFF");
  }
}

String getStatusText(bool state) {
  return state ? "ON" : "OFF";
}

void sendWebPage(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<title>Smart Light Control</title>");
  client.println("<style>");
  client.println("body { font-family: Arial; text-align: center; margin-top: 40px; }");
  client.println(".card { width: 320px; margin: auto; padding: 20px; border: 1px solid #ccc; border-radius: 10px; }");
  client.println("button { width: 220px; padding: 12px; margin: 10px; font-size: 16px; }");
  client.println("p { font-size: 18px; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<div class='card'>");
  client.println("<h2>Linda's Home Light Control</h2>");

  client.print("<p>Living room: ");
  client.print(getStatusText(livingState));
  client.println("</p>");
  client.println("<a href='/toggle?room=living%20room'><button>Toggle Living Room</button></a>");

  client.print("<p>Bathroom: ");
  client.print(getStatusText(bathroomState));
  client.println("</p>");
  client.println("<a href='/toggle?room=bathroom'><button>Toggle Bathroom</button></a>");

  client.print("<p>Closet: ");
  client.print(getStatusText(closetState));
  client.println("</p>");
  client.println("<a href='/toggle?room=closet'><button>Toggle Closet</button></a>");

  client.println("</div>");
  client.println("</body>");
  client.println("</html>");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected.");
    String request = "";
    bool currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (c == '\n' && currentLineIsBlank) {
          // Process request before sending page
          if (request.indexOf("GET /toggle?room=living%20room") >= 0) {
            toggleLight("living room");
          }
          else if (request.indexOf("GET /toggle?room=bathroom") >= 0) {
            toggleLight("bathroom");
          }
          else if (request.indexOf("GET /toggle?room=closet") >= 0) {
            toggleLight("closet");
          }

          sendWebPage(client);
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
