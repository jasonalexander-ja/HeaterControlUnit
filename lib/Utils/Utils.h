#include <Arduino.h>

#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>


#define RELAY_PIN 25
#define PREFS_APP_NAME "heating-control"
#define ON_TIME_SECS 1800
#define MSG_DISPLAY_TIME 2000


String GetCardUidBlocking(MFRC522 *rfid);

void ShowLcdMsg(String line1, String line2, LiquidCrystal_I2C* lcd);
