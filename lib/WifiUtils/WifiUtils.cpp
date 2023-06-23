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

String GetStoredPassword(Preferences* prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	String pswdStr = prefs->getString("PSWD", "");
	if (pswdStr == "") prefs->putString("PSWD", " ");
	prefs->end();
	return pswdStr;
}

String GetStoredSSID(Preferences* prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	String ssidStr = prefs->getString("SSID", "");
	if (ssidStr == "") prefs->putString("SSID", " ");
	prefs->end();
	return ssidStr;
}


bool SetupWifi(Preferences* prefs, LiquidCrystal_I2C* lcd)
{
	String ssid = GetStoredSSID(prefs);
	String pswd = GetStoredPassword(prefs);

	WiFi.begin(ssid.c_str(), pswd.c_str());

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
