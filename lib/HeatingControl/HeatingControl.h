#include <LiquidCrystal_I2C.h>
#include <Preferences.h>

int HandleControlServerResponse(int httpResponse, LiquidCrystal_I2C* lcd, Preferences* prefs);

void CheckRemainingTime(LiquidCrystal_I2C* lcd, Preferences* prefs);

