
## Nottinghack Heater Unit Control Box

This contains the code for the ESP32s inside the heater unit control boxes. 

### Circuit 

#### Components

- [250VAC to 5VDC Power supply](https://www.amazon.co.uk/dp/B074CB1N7Z)
- [ESP32 DevKit V1](https://www.amazon.co.uk/dp/B0C37ZNVQM)
- [YYG-2 Relay Module](https://www.amazon.co.uk/Trigger-Relay-Module-Optocoupler-Isolation/dp/B07TWH7FWW)
- [MFRC522 RFID Module](https://www.amazon.co.uk/dp/B09H6PLM1C)
- [16x2 I2C LCD Module](https://www.amazon.co.uk/dp/B091DYP7NM)


ESP32 to RC522 RFID module:
 - ESP32 G4 -> RC522 RST 
 - ESP32 G5 -> RC522 SDA 
 - ESP32 G18 -> RC522 SCK 
 - ESP32 G19 -> RC522 MISO 
 - ESP32 G32 -> RC522 MOSI 
 - ESP32 3.3v -> RC522 3.3v
 - ESP32 GND -> RC522 GND

ESP32 to I2C LCD Module:
  - ESP32 G22 (SCL) -> LCD SCL
  - ESP32 G21 (SDA) -> LCD SDA

ESP32 -> Power control relay unit:
 - ESP32 G25 -> Switch 'S' pin on relay module 

Power should be tapped off the 5V power supply. 


