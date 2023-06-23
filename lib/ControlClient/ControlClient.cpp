#include <Preferences.h>
#include <HTTPClient.h>

#include <LiquidCrystal_I2C.h>

#include <Utils.h>


int HttpReq(String uri, Preferences *prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	String address = "http://" + prefs->getString("IPAD", "") + uri;
	prefs->end();

	HTTPClient http;
	http.begin(address.c_str());
	return http.GET();
}

bool ClientStatus(Preferences *prefs)
{
	return HttpReq("/Heater/Ping", prefs) == 200;
}

int RequestHeatingHttp(String cardId, Preferences *prefs)
{
	return HttpReq("/Heater/Request/" + cardId, prefs);
}
