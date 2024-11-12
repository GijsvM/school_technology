#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const int luchtSensorPin = 36;
const int vochtigheidSensorPin = 39;
const int pompPin = 25;

const char* ssid = "NETLAB-OIL460";
const char* password = "Startsemester";

const char* serverURL = "http://192.168.68.92:8080/plantData";
const char* serverName = "http://192.168.68.92:8080/plantData/1";

unsigned long lastTime = 0;
unsigned long timerDelay = 11000;
unsigned long sensorReadInterval = 10000;
unsigned long pumpOnDuration = 1000; // Pump active duration in milliseconds
unsigned long lastSensorReadTime = 0;
unsigned long pumpStartTijd = 0;

String sensorReadings;
float sensorReadingsArr[3];
bool pumpActive = false;

String httpGETRequest(const char* serverName);

void setup() {
  pinMode(luchtSensorPin, INPUT);
  pinMode(vochtigheidSensorPin, INPUT);
  pinMode(pompPin, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  unsigned long currentMillis = millis();
  int lucht;
  int vochtigheid;

  // Check if it's time to send HTTP GET request
  if (currentMillis - lastTime > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      sensorReadings = httpGETRequest(serverName);

      if (sensorReadings == "{}" || sensorReadings == "") {
        Serial.println("Empty response received, skipping JSON parsing");
      } else {
        JSONVar myObject = JSON.parse(sensorReadings);

        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
        } else {
          JSONVar keys = myObject.keys();
          
          for (int i = 0; i < keys.length(); i++) {
            JSONVar value = myObject[keys[i]];
            Serial.println(value); 

            if (value) {
              pumpActive = true;
              pumpStartTijd = currentMillis;
              digitalWrite(pompPin, HIGH);
              Serial.println("Pump activated.");
            } else {
              digitalWrite(pompPin, LOW);
              pumpActive = false;
            }
          }
        }
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = currentMillis;
  }

  // Turn off the pump after the duration has elapsed
  if (pumpActive && (currentMillis - pumpStartTijd >= pumpOnDuration)) {
    digitalWrite(pompPin, LOW);
    pumpActive = false;
    Serial.println("Pump deactivated.");
  }

  // Periodic sensor readings and POST request
  if (currentMillis - lastSensorReadTime >= sensorReadInterval) {
    lucht = analogRead(luchtSensorPin);
    vochtigheid = analogRead(vochtigheidSensorPin);  // dry = 1600, in water = 700
    Serial.print("Humidity: ");  
    Serial.println(vochtigheid);
    Serial.print("Air quality: ");  
    Serial.println(lucht, DEC);
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");

      String postData = "{\"plantID\": 1, \"vochtigheid\": " + String(vochtigheid) + ", \"luchtKwaliteid\": " + String(lucht) + " }";
      int httpResponseCode = http.POST(postData);
      Serial.println("POST data: " + postData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastSensorReadTime = currentMillis;
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  return payload;
}
