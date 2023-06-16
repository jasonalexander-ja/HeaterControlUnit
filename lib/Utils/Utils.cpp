#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

void DropCard(MFRC522* rfid)
{
	rfid->PICC_HaltA();
	rfid->PCD_StopCrypto1();
}

bool GetCardUid(MFRC522* rfid, String* uid)
{
	if (!rfid->PICC_IsNewCardPresent()) return false;
	if (!rfid->PICC_ReadCardSerial()) return false;

	MFRC522::PICC_Type piccType = rfid->PICC_GetType(rfid->uid.sak);

	if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
		piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
		piccType != MFRC522::PICC_TYPE_MIFARE_4K
	) return false;

	String cardId = "";
	for (byte i = 0; i < 4; i++)
	{
		cardId = cardId + String(rfid->uid.uidByte[i]);
	}
		
	DropCard(rfid);

	*uid = cardId;
	return true;
}

String GetCardUidBlocking(MFRC522* rfid)
{
	String cardId = "";
	while (!GetCardUid(rfid, &cardId)) {}
	return cardId;
}

void ShowLcdMsg(String line1, String line2, LiquidCrystal_I2C* lcd)
{
	lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print(line1);
	lcd->setCursor(0, 1);
	lcd->print(line2);
}
