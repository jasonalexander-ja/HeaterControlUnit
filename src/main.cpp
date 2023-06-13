#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <MFRC522.h>

#include <WifiUtils.h>
#include <ErrorState.h>
#include <HeatingControl.h>
#include <ControlClient.h>
#include <Utils.h>

Preferences preferences;

MFRC522 rfid(5, 4);

void setup()
{
	Serial.begin(115200);
	
	SPI.begin();
	rfid.PCD_Init();

	pinMode(RELAY_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);

	if (!SetupWifi(&preferences)) 
		ErrorLoopBlocking(&preferences);
	
	if (!ClientStatus(&preferences)) 
		ErrorLoopBlocking(&preferences);
}

void loop()
{
	// When ready to accept a card, show the LED 
	digitalWrite(LED_PIN, HIGH);

	String cardId = GetCardUidBlocking(&rfid);

	int res = RequestHeatingHttp(&preferences, cardId);
	
	if (HandleControlServerResponse(res) == -1)
		ErrorLoopBlocking(&preferences);
}
