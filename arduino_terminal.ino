#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "AnalogKeyboard.h"
#include "KeyboardConverter.h"
#include "DisplayKeyboardProcessor.h"

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          7         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27,16,2); 
KeyboardConverter kc;
DisplayKeyboardProcessor dkp(lcd, mfrc522, 5);

AnalogKeyboard kbd(A1);

void setup() {
  lcd.init();
  lcd.print("Initializing rdr");
  lcd.backlight();
  Serial.begin(9600);   // Initialize serial communications with the PC
  Serial1.begin(9600);
  SPI.begin();      // Init SPI bus
  
  mfrc522.PCD_Init();   // Init MFRC522
  
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  
  lcd.clear();
  lcd.setCursor(0,0);
  dkp.init();
  kbd.connectTo(kc);
  kc.connectTo(dkp);
}

char * byteToHex(byte b) {
  const char * digits = "01234567890ABCDEF";
  char * res = new char[3];
  res[0] = digits[b & 15];
  res[1] = digits[(b & 240) >> 4];
  res[2] = 0;

  return res;
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  /*if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Card ID:");
  lcd.setCursor(0,1);

  for (int i = 0; i < mfrc522.uid.size; ++i) {
    char * t = byteToHex(mfrc522.uid.uidByte[i]);
    Serial1.println(t);
    lcd.setCursor(i *2, 1);
    lcd.print(t);
    delete[] t;
  }*/
  kbd.loop();
  dkp.loop();
}