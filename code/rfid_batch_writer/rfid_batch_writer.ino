/* ESP32C6-based Batch Card Writer for Grimmboy - Arduino-controlled kids' audio player
 * Adapted by Angelina Krinos for Stanford Lab64 Valentine's Day Workshop.
 * Inspired by Turi Scandurra's repo.
 *
 * This code writes sequential track numbers onto MIFARE Classic tags using MFRC522v2.
 */

#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// Pin definitions for ESP32C6
#define SCK_PIN   19  // SPI Clock
#define MISO_PIN  20  // SPI MISO
#define MOSI_PIN  18  // SPI MOSI
#define SS_PIN    2   // MFRC522 SDA (SS)
#define RST_PIN   1   // MFRC522 RST (Optional)

MFRC522DriverPinSimple ss_pin(SS_PIN);
MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
MFRC522 rfid{driver}; // Create MFRC522 instance

MFRC522::MIFARE_Key key;
byte blockAddress = 2;
byte newBlockData[16] = {"Track 0001"}; // Default track number
byte bufferblocksize = 18;
byte blockDataRead[18];
uint16_t currentNumber = 1; // Start track number

void setup() {
  Serial.begin(9600);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  rfid.PCD_Init();
  Serial.println(F("RFID Writer Initialized. Warning: This will overwrite data on your card!"));
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  Serial.print("Card UID: ");
  MFRC522Debug::PrintUID(Serial, (rfid.uid));
  Serial.println();

  if (rfid.PCD_Authenticate(0x60, blockAddress, &key, &(rfid.uid)) != 0) {
    Serial.println("Authentication failed.");
    return;
  }

  // Convert track number to ASCII
  sprintf((char*)newBlockData, "Track %04d", currentNumber);
  
  if (rfid.MIFARE_Write(blockAddress, newBlockData, 16) != 0) {
    Serial.println("Write failed.");
  } else {
    Serial.print("Data written successfully in block: ");
    Serial.println(blockAddress);
    
    // Verify by reading back the data
    if (rfid.MIFARE_Read(blockAddress, blockDataRead, &bufferblocksize) != 0) {
      Serial.println("Verification read failed.");
    } else {
      Serial.println("Verification read successful!");
      Serial.print("Data in block ");
      Serial.print(blockAddress);
      Serial.print(": ");
      for (byte i = 0; i < 16; i++) {
        Serial.print((char)blockDataRead[i]);
      }
      Serial.println();
    }
  }

  currentNumber++; // Increment track number
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(2000);
}
