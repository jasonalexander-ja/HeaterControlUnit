#include <Preferences.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Utils.h>

bool ClientStatus(Preferences *prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	HTTPClient http;
	String address = "http://" + prefs->getString("IPAD", "") + "/Heater/Ping";
	http.begin(address.c_str());
	int responseCode = http.GET();
	prefs->end();
	return responseCode == 200;
}

int RequestHeatingHttp(String cardId, Preferences *prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	String address = "http://" + prefs->getString("IPAD", "") + "/Heater/Request/" + cardId;
	prefs->end();

	HTTPClient http;
	http.begin(address.c_str());
	return http.GET();
}
