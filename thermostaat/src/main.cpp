#include <Arduino.h>
#include <DHT.h>
#include "Display.h"
int dhtPin = 12;
bool HumShow;
bool TempShow;
#define DHTTYPE DHT11 // DHT 11 
int buttonstate1 = 0;
int buttonstate2 = 0;
DHT dht(dhtPin, DHTTYPE);
void setup() {
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
pinMode(7,OUTPUT);
pinMode(8,INPUT_PULLUP);
pinMode(9,INPUT_PULLUP);
dht.begin();
}

void loop() {

buttonstate1 = digitalRead(8);
buttonstate2 = digitalRead(9);

float h = dht.readHumidity();

float t = dht.readTemperature();

if(buttonstate1 == LOW){
   TempShow = true;
   HumShow = false;
}
if(buttonstate2 == LOW){
   TempShow = false;
   HumShow = true;
}
if (TempShow){
Display.show(t);
delay(100);

}
if (HumShow){
Display.show(h);
delay(100);

}



if(t<10.00){
  digitalWrite(4,HIGH);
  delay(200);
  digitalWrite(4,LOW);
  delay(200);
}
if(t>30.00){
  digitalWrite(4,HIGH);
  delay(200);
  digitalWrite(4,LOW);
  delay(200);
}

if(h < 20.00){
  digitalWrite(4,HIGH);
  delay(200);
  digitalWrite(4,LOW);
  delay(200);
}
if(h > 70.00){
  digitalWrite(7,HIGH);
  delay(200);
  digitalWrite(7,LOW);
  delay(200);
}

}
