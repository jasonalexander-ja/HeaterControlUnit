#include <MFRC522.h>
#include <Arduino.h>

#define LED_PIN 2
#define RELAY_PIN 25

String GetCardUidBlocking(MFRC522 *rfid);
