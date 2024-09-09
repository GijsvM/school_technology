#include <Arduino.h>
#include "Display.h"
int count = 0;
const int buttonPinMin = 9;
const int buttonPinPlus = 8;
int buttonstateMin = 0;
int buttonstatePlus = 0;
void setup() {
pinMode(buttonPinMin, INPUT_PULLUP);
pinMode(buttonPinPlus, INPUT_PULLUP);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
pinMode(7,OUTPUT);

}

void loop() {
buttonstateMin = digitalRead(buttonPinMin);
buttonstatePlus = digitalRead(buttonPinPlus);

if(buttonstateMin == LOW){
  count--;
  delay(200);
  Display.show(count); 
}
if(buttonstatePlus == LOW){
  count++;
  delay(200);
  Display.show(count); 
}
if(count > 99993){
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(200);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(200);
}
}
