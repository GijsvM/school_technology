#include <Arduino.h>
float num1;
float num2;
char oper;
float antwoord;

void setup()
{
    Serial.begin(9600); // begins serial  communications
    Serial.println("typ de berekening");
    Serial.println("b.v.b: 2+3\n");
}
void calc()
{

    switch (oper)
    {

    case '+':
        antwoord = num1 + num2;
        break;

    case '-':
        antwoord = num1 - num2;
        break;

    case '*':
        antwoord = num1 * num2;
        break;

    case '/':
        antwoord = num1 / num2;
        break;
    }
}

void loop()
{
    while (Serial.available() > 0)
    {
        num1 = Serial.parseFloat();
        oper = Serial.read();
        num2 = Serial.parseFloat();

        calc();

        Serial.println("het antwoord is: ");
        Serial.println(antwoord);
        Serial.println();
    }
}
