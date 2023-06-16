#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include <LiquidCrystal_I2C.h>

#include <WifiUtils.h>
#include <ControlClient.h>
#include <Utils.h>

String ReadLineBlocking(LiquidCrystal_I2C* lcd)
{
	ShowLcdMsg("Type HELP for a", "list of commands", lcd);
	while (Serial.available() == 0) {}
	return Serial.readStringUntil('\n');
}

String ReadNextLineBlocking(String line, LiquidCrystal_I2C* lcd)
{
	ShowLcdMsg(line, "", lcd);
	while (Serial.available() == 0) {}
	return Serial.readStringUntil('\n');
}

void WaitForSerial(LiquidCrystal_I2C* lcd)
{
	ShowLcdMsg("Error. Connect a", "USB serial @9600", lcd);
	while (!Serial) {}
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

bool ProcessMessage(String command, Preferences* prefs, LiquidCrystal_I2C* lcd)
{
	String caseInsensitive = String(command);
	caseInsensitive.toUpperCase();
	prefs->begin(PREFS_APP_NAME, false);
	if (command == "STAT")
		Serial.println("SETUP");

	else if (command == "SET_SSID")
		prefs->putString("SSID", ReadNextLineBlocking("Send SSID", lcd));

	else if (command == "SET_PSWD")
		prefs->putString("PSWD", ReadNextLineBlocking("Send password", lcd));

	else if (command == "SET_IPAD")
		prefs->putString("IPAD", ReadNextLineBlocking("Send server IP", lcd));

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
		if (SetupWifi(prefs, lcd))
		{
			if (ClientStatus(prefs)) 
			{
				Serial.println("CONNECTED");
				ShowLcdMsg("Connected", "", lcd);
				delay(MSG_DISPLAY_TIME);
				return true;
			}
			Serial.println("FAILED SERVER");
				ShowLcdMsg("Failed to ping", "control server. ", lcd);
				delay(MSG_DISPLAY_TIME);
			return false;
		}
		Serial.println("FAILED CONNECT");
		ShowLcdMsg("Couldn't connect", "to network. ", lcd);
		delay(MSG_DISPLAY_TIME);
		return false;
	}
	prefs->end();
	return false;
}

void ErrorLoopBlocking(Preferences* prefs, LiquidCrystal_I2C* lcd)
{
	WaitForSerial(lcd);
	while (true)
	{
		String command = ReadLineBlocking(lcd);
		if (ProcessMessage(command, prefs, lcd))
			break;
	}
}
