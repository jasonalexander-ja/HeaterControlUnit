#include <Arduino.h>
#include <Preferences.h>

#include <ErrorState.h>

void MainHeatingLoop()
{
	digitalWrite(25, HIGH);
	digitalWrite(2, LOW);
	delay(1800000);
	digitalWrite(2, HIGH);
	digitalWrite(25, LOW);
}

void DeniedLoop()
{
	digitalWrite(2, LOW);
	delay(250);
	digitalWrite(2, HIGH);
	delay(250);
	digitalWrite(2, LOW);
	delay(250);
	digitalWrite(2, HIGH);
}

int HandleControlServerResponse(int httpResponse)
{
	switch (httpResponse)
	{
	case 200:
		MainHeatingLoop();
		return 0;
	case 403:
		DeniedLoop();
		return 0;
	default:
		return -1;
	}
}
