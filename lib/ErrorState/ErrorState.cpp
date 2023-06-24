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

String ReadLineWithMsgBlocking(String message, LiquidCrystal_I2C* lcd)
{
	ShowLcdMsg(message, "", lcd);
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

void ProcessSetters(String command, Preferences* prefs, LiquidCrystal_I2C* lcd)
{
	prefs->begin(PREFS_APP_NAME, false);

	if (command == "SET_SSID")
		prefs->putString("SSID", ReadLineWithMsgBlocking("Send SSID", lcd));

	else if (command == "SET_PSWD")
		prefs->putString("PSWD", ReadLineWithMsgBlocking("Send password", lcd));

	else if (command == "SET_IPAD")
		prefs->putString("IPAD", ReadLineWithMsgBlocking("Send server IP", lcd));

	prefs->end();
}

void ProcessGetters(String command, Preferences* prefs)
{
	prefs->begin(PREFS_APP_NAME, false);
	if (command == "GET_SSID")
		Serial.println(prefs->getString("SSID", ""));

	else if (command == "GET_PSWD")
		Serial.println(prefs->getString("PSWD", ""));

	else if (command == "GET_IPAD")
		Serial.println(prefs->getString("IPAD", ""));
	prefs->end();
}

void ProcessHelpMessage(String command)
{
	String caseInsensitive = String(command);
	caseInsensitive.toUpperCase();
	if (caseInsensitive == "HELP" || caseInsensitive == "H") 
		HelpMessage();
}

void ConnectMessage(String serialMsg, String upperLcd, String lowerLcd, LiquidCrystal_I2C* lcd)
{
	Serial.println(serialMsg);
	ShowLcdMsg(upperLcd, lowerLcd, lcd);
	delay(MSG_DISPLAY_TIME);
}

bool ConnectAndCheckNetwork(Preferences* prefs, LiquidCrystal_I2C* lcd)
{
	if (SetupWifi(prefs, lcd))
	{
		if (ClientStatus(prefs)) 
		{
			ConnectMessage("CONNECTED", "Connected", "", lcd);
			return true;
		}
		ConnectMessage("FAILED SERVER", "Failed to ping", "control server. ", lcd);
		return false;
	}
	ConnectMessage("FAILED CONNECT", "Couldn't connect", "to network. ", lcd);
	return false;
}

bool ProcessMessage(String command, Preferences* prefs, LiquidCrystal_I2C* lcd)
{
	if (command == "STAT")
		Serial.println("SETUP");

	ProcessSetters(command, prefs, lcd);
	ProcessGetters(command, prefs);
	ProcessHelpMessage(command);

	if (command == "CONNECT")
		return ConnectAndCheckNetwork(prefs, lcd);

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
