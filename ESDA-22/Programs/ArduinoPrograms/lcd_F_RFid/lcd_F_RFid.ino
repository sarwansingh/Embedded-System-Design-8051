#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

const int rs = 5, en = 3, d4 = 2, d5 = A2, d6 = 15, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  lcd.print(" SPI = RFid * ");
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init(); 
 
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
}
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        lcd.print(buffer[i] < 0x10 ? " 0" : " ");
        lcd.print(buffer[i], HEX);    
    }
}

void loop() {
  lcd.setCursor(0,1);
 // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  delay(2000);
}
