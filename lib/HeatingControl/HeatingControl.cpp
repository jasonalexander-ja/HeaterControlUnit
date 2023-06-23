#include <Arduino.h>
#include <Preferences.h>

#include <LiquidCrystal_I2C.h>

#include <ErrorState.h>
#include <Utils.h>


void UpdatePrefsTime(int timeRemaining, Preferences* prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	prefs->putInt("TIME", timeRemaining);
	prefs->end();
}

void PrintTime(int timeSecs, LiquidCrystal_I2C* lcd)
{
	String mins = String(timeSecs / 60);
	if (mins.length() == 1) mins = "0" + mins;

	String secs = String(timeSecs % 60);
	if (secs.length() == 1) secs = "0" + secs;

	String msg = mins + ":" + secs;
	lcd->setCursor(11, 0);
	lcd->print(msg);
}

void MainHeatingLoop(int time, LiquidCrystal_I2C* lcd, Preferences* prefs)
{
	ShowLcdMsg("Time left: ", "", lcd);
	digitalWrite(RELAY_PIN, HIGH);
	for (int count = time; count > 0; --count)
	{
		UpdatePrefsTime(count, prefs);
		PrintTime(count, lcd);
		delay(1000);
	}
	UpdatePrefsTime(0, prefs);
	digitalWrite(RELAY_PIN, LOW);
}

void DeniedLoop(LiquidCrystal_I2C* lcd)
{
	ShowLcdMsg("Request blocked", "Check HMS", lcd);
	delay(MSG_DISPLAY_TIME);
}

int HandleControlServerResponse(int httpResponse, LiquidCrystal_I2C* lcd, Preferences* prefs)
{
	switch (httpResponse)
	{
	case 200:
		MainHeatingLoop(ON_TIME_SECS, lcd, prefs);
		return 0;
	case 403:
		DeniedLoop(lcd);
		return 0;
	default:
		return -1;
	}
}

void CheckRemainingTime(LiquidCrystal_I2C* lcd, Preferences* prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	int timeRemaining = prefs->getInt("TIME", 0);
	prefs->end();
	if (timeRemaining > 0) MainHeatingLoop(timeRemaining, lcd, prefs);
}
