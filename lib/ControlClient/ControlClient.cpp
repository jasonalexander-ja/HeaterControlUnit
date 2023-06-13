#include <Preferences.h>
#include <HTTPClient.h>

bool ClientStatus(Preferences *prefs)
{
	prefs->begin("heating-control", false);
	HTTPClient http;
	String address = "http://" + prefs->getString("IPAD", "") + "/Heater/Ping";
	http.begin(address.c_str());
	int responseCode = http.GET();
	prefs->end();
	return responseCode == 200;
}

int RequestHeatingHttp(Preferences *prefs, String cardId)
{
	prefs->begin("heating-control", false);
	HTTPClient http;
	String address = "http://" + prefs->getString("IPAD", "") + "/Heater/Request/" + cardId;
	http.begin(address.c_str());
	prefs->end();
	return http.GET();
}
