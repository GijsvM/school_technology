#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}
void loop()
{

  static float time = millis();
  float myTime = millis();
  int myTimeSec = myTime / 1000;
  int myTimemin = myTimeSec / 60;
  int myTimeHour = myTimemin / 60;
  int myTimeDay = myTimeHour / 24;

  if (myTimemin >= 1)
  {
    digitalWrite(4, HIGH);
  }
  if (myTimemin >= 2)
  {
    digitalWrite(5, HIGH);
  }
  if (myTimemin >= 3)
  {
    digitalWrite(6, HIGH);
  }
  if (myTimemin >= 4)
  {
    digitalWrite(7, HIGH);
  }
  while (myTimeSec >= 60)
  {
    myTimeSec -= 60;
  }
  while (myTimemin >= 60)
  {
    myTimemin -= 60;
  }
  while (myTimeHour >= 24)
  {
    myTimeHour -= 60;
  }

  if (time + 1000 <= millis())
  {
    time = millis();
    Serial.write(0x1a);
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nuptime:");
    Serial.print("sec: ");
    Serial.println(myTimeSec);
    Serial.print("min: ");
    Serial.println(myTimemin);
    Serial.print("hour: ");
    Serial.println(myTimeHour);

    Serial.print("day: ");
    Serial.println(myTimeDay);
  }
}