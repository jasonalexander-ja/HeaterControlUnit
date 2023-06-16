#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

void DropCard(MFRC522* rfid)
{
	rfid->PICC_HaltA();
	rfid->PCD_StopCrypto1();
}

bool CheckPiccType(MFRC522* rfid)
{
	MFRC522::PICC_Type piccType = rfid->PICC_GetType(rfid->uid.sak);
	return piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
		piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
		piccType != MFRC522::PICC_TYPE_MIFARE_4K;
}

String CopyUidString(MFRC522* rfid)
{
	String cardId = "";
	for (byte i = 0; i < 4; i++)
	{
		cardId = cardId + String(rfid->uid.uidByte[i]);
	}
	return cardId;
}

bool GetCardUid(MFRC522* rfid, String* uid)
{
	if (!rfid->PICC_IsNewCardPresent()) return false;
	if (!rfid->PICC_ReadCardSerial()) return false;

	if (CheckPiccType(rfid)) return false;

	*uid = CopyUidString(rfid);
	
	DropCard(rfid);
	
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
	lcd->print(line1.substring(0, 16));
	lcd->setCursor(0, 1);
	lcd->print(line2.substring(0, 16));
}
