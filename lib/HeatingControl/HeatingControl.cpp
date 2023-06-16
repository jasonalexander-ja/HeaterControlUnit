#include <Arduino.h>
#include <Preferences.h>

#include <LiquidCrystal_I2C.h>

#include <ErrorState.h>
#include <Utils.h>

void MainHeatingLoop(LiquidCrystal_I2C* lcd)
{
	ShowLcdMsg("Time left: ", "", lcd);
	digitalWrite(RELAY_PIN, HIGH);
	for (int count = ON_TIME_SECS; count > 0; --count)
	{
		String mins = String(count / 60);
		if (mins.length() == 1) mins = "0" + mins;

		String secs = String(count % 60);
		if (secs.length() == 1) secs = "0" + secs;

		String msg = mins + ":" + secs;
		lcd->setCursor(11, 0);
		lcd->print(msg);
		delay(1000);
	}
	digitalWrite(RELAY_PIN, LOW);
}

void DeniedLoop(LiquidCrystal_I2C* lcd)
{
	ShowLcdMsg("Request blocked", "Check HMS", lcd);
	delay(MSG_DISPLAY_TIME);
}

int HandleControlServerResponse(int httpResponse, LiquidCrystal_I2C* lcd)
{
	switch (httpResponse)
	{
	case 200:
		MainHeatingLoop(lcd);
		return 0;
	case 403:
		DeniedLoop(lcd);
		return 0;
	default:
		return -1;
	}
}
