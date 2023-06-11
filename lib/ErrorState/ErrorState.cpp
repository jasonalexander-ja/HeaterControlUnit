#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include <WifiUtils.h>
#include <ControlClient.h>

String ReadLineBlocking()
{
    while (Serial.available() == 0);
    return Serial.readStringUntil('\n');
}

bool ProcessMessage(Preferences prefs, String command)
{
	prefs.begin("heating-control", false);
    if (command == "STAT")
        Serial.println("SETUP");
    
    else if (command == "SET_SSID")
        prefs.putString("SSID", ReadLineBlocking());
    else if (command == "SET_PSWD")
        prefs.putString("PSWD", ReadLineBlocking());
    else if (command == "SET_IPAD")
        prefs.putString("IPAD", ReadLineBlocking());
    
    else if (command == "GET_SSID")
        Serial.println(prefs.getString("SSID", ""));

    else if (command == "GET_PSWD")
        Serial.println(prefs.getString("PSWD", ""));

    else if (command == "GET_IPAD")
        Serial.println(prefs.getString("IPAD", ""));
    
    else if (command == "CONNECT")
    {
		prefs.end();
		if(SetupWifi(prefs))
        {
			if (ClientStatus(prefs)) return true;
			else Serial.println("Failed to reach control client. ");
        } else Serial.println("Failed to connect to network. ");
        Serial.println("FAILED");
    }
    prefs.end();
    return false;
}

void ErrorLoopBlocking(Preferences prefs) 
{
	int count = 0;
	int state = HIGH;
	while (true)
    {
		if (count == 1000000) 
		{
			count = 0;
			state = !state;
			digitalWrite(9, state);
		}
        String command = ReadLineBlocking();
        if (ProcessMessage(prefs, command))
            break;
    }

    digitalWrite(9, LOW);
}
