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

Preferences preferences;

MFRC522 rfid(5, 4);

void setup()
{
	Serial.begin(115200);
	while (!Serial);
	SPI.begin();
	rfid.PCD_Init();
	pinMode(25, OUTPUT);
	pinMode(2, OUTPUT);
	if(!SetupWifi(preferences)) ErrorLoopBlocking(preferences);
	if(!ClientStatus(preferences)) ErrorLoopBlocking(preferences);

	digitalWrite(2, HIGH);
}

void DropCard()
{
	rfid.PICC_HaltA();
	rfid.PCD_StopCrypto1();
}

void loop()
{
	if (!rfid.PICC_IsNewCardPresent())
		return;

	if (!rfid.PICC_ReadCardSerial())
		return;

	MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

	if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
		piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
		piccType != MFRC522::PICC_TYPE_MIFARE_4K
	) return;

	String cardId = "";
	for (byte i = 0; i < 4; i++)
	{
		cardId = cardId + String(rfid.uid.uidByte[i]);
	}

	int res = RequestHeatingHttp(preferences, cardId);
	`
	if (res == 200) 
	{
		DropCard();
		MainHeatingLoop();
		return;
	}
	else if (res == 403) 
	{
		DropCard();
		DeniedLoop();
		return;
	}
	DropCard();
	if (!ClientStatus(preferences))
		ErrorLoopBlocking(preferences);
}
