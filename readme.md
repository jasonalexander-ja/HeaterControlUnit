
## Nottinghack Heater Unit Control Box

This contains the code for the ESP32s inside the heater unit control boxes. 

### Circuit 

ESP32 to RC522 RFID module:
 - ESP32 G0 -> RC522 RST 
 - ESP32 G5 -> RC522 SDA 
 - ESP32 G18 -> RC522 SCK 
 - ESP32 G19 -> RC522 MISO 
 - ESP32 G32 -> RC522 MOSI 
 - ESP32 3.3v -> RC522 3.3v
 - ESP32 GND -> RC522 GND

ESP32 -> Power control relay unit:
 - ESP32 G2 -> Switch 'S' pin on relay module 

