#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include <ControlClient.h>

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

bool SetupWifi(Preferences* prefs)
{
	prefs->begin("heating-control", false);
	String ssidStr = prefs->getString("SSID", "");
	String pswdStr = prefs->getString("PSWD", "");
	if (ssidStr == "") prefs->putString("SSID", " ");
	if (pswdStr == "") prefs->putString("PSWD", " ");

	char ssid[51];
	char pswd[51];

	prefs->getString("SSID", "").toCharArray(ssid, 50);
	prefs->getString("PSWD", "").toCharArray(pswd, 50);

	WiFi.begin(ssid, pswd);

	while (WiFi.status() != WL_CONNECTED)
	{
		if (!WifiStatusOk())
		{
			prefs->end();
			return false;
		}
	}
	prefs->end();
	return true;
}
