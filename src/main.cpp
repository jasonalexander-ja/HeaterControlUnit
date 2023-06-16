#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#include <WifiUtils.h>
#include <ErrorState.h>
#include <HeatingControl.h>
#include <ControlClient.h>
#include <Utils.h>

Preferences preferences;

MFRC522 rfid(5, 4);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
	Serial.begin(9600);
	
	SPI.begin();
	rfid.PCD_Init();
	lcd.init();

	pinMode(RELAY_PIN, OUTPUT);
	
	lcd.clear();
	lcd.backlight();

	if (!SetupWifi(&preferences, &lcd)) 
		ErrorLoopBlocking(&preferences, &lcd);
	
	if (!ClientStatus(&preferences)) 
	{
		ShowLcdMsg("Error pinging", "server. ", &lcd);
		ErrorLoopBlocking(&preferences, &lcd);
	}
}

void loop()
{
	ShowLcdMsg("Ready for card", "", &lcd);
	String cardId = GetCardUidBlocking(&rfid);

	ShowLcdMsg("Request sent...", "", &lcd);
	int res = RequestHeatingHttp(cardId, &preferences);
	
	if (HandleControlServerResponse(res, &lcd) == -1)
	{
		ErrorLoopBlocking(&preferences, &lcd);
	}
}
