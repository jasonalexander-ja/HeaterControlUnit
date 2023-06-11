#include <Arduino.h>

void MainHeatingLoop()
{
    digitalWrite(2, HIGH);
    delay(1800000);
    digitalWrite(2, LOW);
}

void DeniedLoop()
{
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(500);
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
}
