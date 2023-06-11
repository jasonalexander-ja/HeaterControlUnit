#include <Preferences.h>
#include <HTTPClient.h>

bool ClientStatus(Preferences prefs) 
{
    HTTPClient http;
    String address = "http://" + prefs.getString("IPAD", "") + "/ping";
    http.begin(address.c_str());
    int responseCode = http.GET();
    return responseCode == 200;
}

int RequestHeatingHttp(Preferences prefs, String cardId)
{
    HTTPClient http;
    String address = "http://" + prefs.getString("IPAD", "") + "/request/" + cardId;
    http.begin(address.c_str());
    return http.GET();
}
