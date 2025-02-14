/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read a new NUID from a PICC (RFID tag/card)
 * and print it to the Serial Monitor.
 *
 * Adapted for an ESP32-C6 with the following pin assignments:
 *   - MFRC522 SDA (SS):  GPIO 3
 *   - MFRC522 RST:       GPIO 2
 *   - SPI SCK:           GPIO 18
 *   - SPI MISO:          GPIO 19
 *   - SPI MOSI:          GPIO 23
 *
 * This sketch detects an RFID tag, reads its UID, and prints the UID and PICC type.
 * (Works with MIFARE Classic tags only.)
 * --------------------------------------------------------------------------------------------------------------------
 */

#include <SPI.h>
#include <MFRC522.h>

#define SCK_PIN   19  // SPI Clock
#define MISO_PIN  20  // SPI MISO
#define MOSI_PIN  18  // SPI MOSI
#define SS_PIN    2   // MFRC522 SDA (SS)
#define RST_PIN   1   // MFRC522 RST (Optional)

// Create an instance of the MFRC522 class using our defined SS and RST pins
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key; 

// Array to store the new NUID (UID) from a card
byte nuidPICC[4];

void setup() { 
  Serial.begin(9600);
  
  // Initialize the SPI bus with our custom SPI pins and set SS to our MFRC522 SS pin (GPIO 3)
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  
  // Initialize the MFRC522 RFID reader (ensure you call this on the rfid instance)
  rfid.PCD_Init(); 

  // Set up the default key bytes (for MIFARE Classic tags)
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scans for MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
}
 
void loop() {
  // Optional debug output to confirm the loop is running
  // Serial.println("Waiting for card...");
  
  // Exit early if no new card is present
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Exit early if unable to read the card's UID
  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check if the PICC is a MIFARE Classic type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  // If the UID is new (different from the previously stored one), print it
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
      rfid.uid.uidByte[1] != nuidPICC[1] || 
      rfid.uid.uidByte[2] != nuidPICC[2] || 
      rfid.uid.uidByte[3] != nuidPICC[3]) {
    Serial.println(F("A new card has been detected."));
    
    // Store the new UID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  } else {
    Serial.println(F("Card read previously."));
  }

  // Halt PICC and stop encryption on the PCD
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

/**
 * Helper routine to print a byte array as hex values to Serial.
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to print a byte array as decimal values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
