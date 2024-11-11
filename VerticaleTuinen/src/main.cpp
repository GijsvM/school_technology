#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const int luchtSensorPin = 36;
const int vochtigheidSensorPin = 39;
const int pompPin = 26;

const char* ssid = "NETLAB-OIL460";
const char* password = "Startsemester";

const char* serverURL = "http://192.168.68.92:8080/plantData";
const char* serverName = "http://192.168.68.92:8080/plantData/1";

unsigned long lastTime = 0;
unsigned long timerDelay = 11000;
String sensorReadings;
float sensorReadingsArr[3];

// Function prototype for httpGETRequest
String httpGETRequest(const char* serverName);

void setup() {
  pinMode(luchtSensorPin, INPUT);
  pinMode(vochtigheidSensorPin, INPUT);
  pinMode(pompPin, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int tijd = millis();
  int lucht;
  int vochtigheid;

  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      sensorReadings = httpGETRequest(serverName);
      
      // Check if sensorReadings is empty
      if (sensorReadings == "{}" || sensorReadings == "") {
        Serial.println("Empty response received, skipping JSON parsing");
        return;
      }
      
      JSONVar myObject = JSON.parse(sensorReadings);
  
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      JSONVar keys = myObject.keys();
    
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.println(value); 
        if (value) {
          // Uncomment below to activate pump
          digitalWrite(pompPin, HIGH);
          Serial.println("Pump activated.");
          delay(1000);
          digitalWrite(pompPin,LOW);
        } else {
          // Uncomment below to deactivate pump
          digitalWrite(pompPin, LOW);
        }
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

  if (tijd % 10000 == 0) {
    lucht = analogRead(luchtSensorPin);
    vochtigheid = analogRead(vochtigheidSensorPin);  // dry = 1600, in water = 700
    Serial.print("Humidity: ");  
    Serial.println(vochtigheid);
    Serial.print("Air quality: ");  
    Serial.println(lucht, DEC);
    Serial.println();
    Serial.println(tijd);
    Serial.println();
    delay(100);
  }

  if (tijd % 10000 == 0) {
    if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi connection status
      HTTPClient http;

      http.begin(serverURL); // Specify request destination
      http.addHeader("Content-Type", "application/json"); // Specify content-type header

      // Data to send in the POST request body
      String postData = "{\"plantID\": 1, \"vochtigheid\": " + String(vochtigheid) + ", \"luchtKwaliteid\": " + String(lucht) + " }";
      int httpResponseCode = http.POST(postData); // Send HTTP POST request
      Serial.println("POST data: " + postData);

      // Check the response code
      if (httpResponseCode > 0) {
        String response = http.getString(); // Get the response to the request
        Serial.println(httpResponseCode);    // Print the response code
        Serial.println(response);            // Print the response payload
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }

      http.end(); // End the HTTP request
      Serial.println();
      Serial.println(tijd);
      Serial.println();
      delay(100);
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}

// Definition for httpGETRequest function
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
