#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include <WifiUtils.h>
#include <ControlClient.h>

String ReadLineBlocking()
{
	int count = 0;
	int state = HIGH;
	while (Serial.available() == 0)
	{
		count++;
		if (count == 100000)
		{
			count = 0;
			state = !state;
			digitalWrite(2, state);
		}
	}
	return Serial.readStringUntil('\n');
}

void WaitForSerial()
{
	int count = 0;
	int state = HIGH;
	while (!Serial) 
	{
		count++;
		if (count == 100000)
		{
			count = 0;
			state = !state;
			digitalWrite(2, state);
		}
	}
}

void HelpMessage()
{
	Serial.println("List of commands: ");
	Serial.println();
	Serial.println("STAT - 'Status' this will return 'SETUP', just as a way of making sure serial is responding correctly. ");
	Serial.println();
	Serial.println("HELP or H - (case insensitive) displays a help message for a list of commands. ");
	Serial.println();
	Serial.println("SET_SSID\\n[WiFi network name]\\n - Sets the name of the WiFi network to connect to. ");
	Serial.println("SET_PSWD\\n[WiFi network password]\\n - Sets the password of the WiFi network to connect to. ");
	Serial.println("SET_IPAD\\n[Client IP address]\\n - Sets the IP address of the control server on the WiFi network. ");
	Serial.println();
	Serial.println("GET_SSID\\n - Gets and prints the stored name of the WiFi network to connect to. ");
	Serial.println("GET_PSWD\\n - Gets and prints the stored password of the WiFi network to connect to. ");
	Serial.println("GET_IPAD\\n - Gets and prints the stored IP address of the control server on the WiFi network. ");
	Serial.println();
	Serial.println("CONNECT\\n - Try to connect to the WiFi network given, and pings the given server IP address, the response will either be");
	Serial.println("	 - CONNECTED - Connected to WiFi network and sucessfully pinged server. ");
	Serial.println("	 - FAILED SERVER - Connnected to WiFi network but failed to ping server. ");
	Serial.println("	 - FAILED CONNECT - Incorrect WiFi name or password. ");
	Serial.println();
	Serial.println(" *'\\n' represents a new line, commands and data are parsed via new lines.*");
	Serial.println(" *All commands must be in upper case.*");
}

bool ProcessMessage(Preferences* prefs, String command)
{
	String caseInsensitive = String(command);
	caseInsensitive.toUpperCase();
	prefs->begin("heating-control", false);
	if (command == "STAT")
		Serial.println("SETUP");

	else if (command == "SET_SSID")
		prefs->putString("SSID", ReadLineBlocking());

	else if (command == "SET_PSWD")
		prefs->putString("PSWD", ReadLineBlocking());

	else if (command == "SET_IPAD")
		prefs->putString("IPAD", ReadLineBlocking());

	else if (command == "GET_SSID")
		Serial.println(prefs->getString("SSID", ""));

	else if (command == "GET_PSWD")
		Serial.println(prefs->getString("PSWD", ""));

	else if (command == "GET_IPAD")
		Serial.println(prefs->getString("IPAD", ""));

	else if (caseInsensitive == "HELP" || caseInsensitive == "H") 
		HelpMessage();

	else if (command == "CONNECT")
	{
		prefs->end();
		if (SetupWifi(prefs))
		{
			if (ClientStatus(prefs)) 
			{
				Serial.println("CONNECTED");
				return true;
			}
			Serial.println("FAILED SERVER");
			return false;
		}
		Serial.println("FAILED CONNECT");
		return false;
	}
	prefs->end();
	return false;
}

void ErrorLoopBlocking(Preferences* prefs)
{
	WaitForSerial();
	while (true)
	{
		String command = ReadLineBlocking();
		if (ProcessMessage(prefs, command))
			break;
	}
	digitalWrite(2, HIGH);
}
