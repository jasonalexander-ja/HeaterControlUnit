#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include <LiquidCrystal_I2C.h>

#include <ControlClient.h>
#include <Utils.h>

bool WifiStatusOk()
{
	switch (WiFi.status())
	{
	case WL_NO_SHIELD:
		return false;
	case WL_NO_SSID_AVAIL:
		return false;
	case WL_CONNECT_FAILED:
		return false;
	case WL_CONNECTION_LOST:
		return false;
	default:
		return true;
	}
}

bool SetupWifi(Preferences* prefs, LiquidCrystal_I2C* lcd)
{
	prefs->begin(PREFS_APP_NAME, false);
	String ssidStr = prefs->getString("SSID", "");
	String pswdStr = prefs->getString("PSWD", "");
	if (ssidStr == "") prefs->putString("SSID", " ");
	if (pswdStr == "") prefs->putString("PSWD", " ");

	char ssid[51];
	char pswd[51];

	prefs->getString("SSID", "").toCharArray(ssid, 50);
	prefs->getString("PSWD", "").toCharArray(pswd, 50);
	prefs->end();

	ShowLcdMsg("Connecting to", ssid, lcd);
	WiFi.begin(ssid, pswd);

	while (WiFi.status() != WL_CONNECTED)
	{
		if (!WifiStatusOk()) 
		{
			ShowLcdMsg("Error connecting", "to WiFi network.", lcd);
			delay(MSG_DISPLAY_TIME);
			return false;
		}
	}
	return true;
}
