/* Lab64 - Valentine's Day Music Box
 * Arduino-controlled music player for a Lab64 workshop
 * Adapted for ESP32C6 by Angelina Krinos
 * Based on work by Turi Scandurra
 * Inspired by Tonuino
 */

#include <Arduino.h>
#include <DFPlayerMini_Fast.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// Pin Definitions for ESP32-C6
#define DFPLAYER_RX 17  // ESP32-C6 RX (DFPlayer TX)
#define DFPLAYER_TX 16  // ESP32-C6 TX (DFPlayer RX)
#define BUSY_PIN 22     // DFPlayer BUSY pin to check playback status
#define BUTTON_PIN 0    // Button for play/pause
#define LED_PIN 21       // LED for status indication

#define SCK_PIN   19    // SPI Clock
#define MISO_PIN  20    // SPI MISO
#define MOSI_PIN  18    // SPI MOSI
#define SS_PIN    2     // MFRC522 SDA (SS)
#define RST_PIN   1     // MFRC522 RST (Optional)

// RFID Initialization
MFRC522DriverPinSimple ss_pin(SS_PIN);
MFRC522DriverSPI driver{ss_pin};
MFRC522 rfid{driver};
MFRC522::MIFARE_Key key;
byte blockAddress = 2;
byte bufferblocksize = 18;
byte blockDataRead[18];

// DFPlayer Mini Initialization
HardwareSerial mySerial(1);
DFPlayerMini_Fast myMP3;

bool isPlaying = false; // Track playback status

void setup() {
  Serial.begin(115200);
  Serial.println("🎵 Initializing Valentine's Day MP3 Player...");

  // DFPlayer Mini setup
  mySerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
  if (!myMP3.begin(mySerial, true)) {
    Serial.println("❌ DFPlayer initialization failed! Check wiring.");
    return;
  }
  Serial.println("✅ DFPlayer Initialized!");
  
  delay(2000); // Allow time for DFPlayer to detect SD card

  pinMode(BUSY_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // SPI and RFID setup
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  rfid.PCD_Init();
  Serial.println(F("✅ RFID Reader Initialized. Ready to scan tags!"));

  // Default authentication key for MIFARE Classic
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void readRFIDTag() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

    Serial.print("📡 Card UID: ");
    MFRC522Debug::PrintUID(Serial, (rfid.uid));
    Serial.println();

    if (rfid.PCD_Authenticate(0x60, blockAddress, &key, &(rfid.uid)) != 0) {
        Serial.println("❌ Authentication failed.");
        return;
    }

    if (rfid.MIFARE_Read(blockAddress, blockDataRead, &bufferblocksize) != 0) {
        Serial.println("❌ Read failed.");
        return;
    }

    // Print raw block data for debugging
    Serial.print("🔍 Raw Block Data: ");
    for (byte i = 0; i < 16; i++) {
        Serial.print(blockDataRead[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // Ensure the block is null-terminated
    blockDataRead[15] = '\0';

    Serial.print("🔍 Interpreted as ASCII: ");
    Serial.println((char*)blockDataRead);

    // Extract numeric portion (assumes format "Track XXXX")
    char trackNumStr[5];  // Buffer for "XXXX" + null terminator
    memcpy(trackNumStr, &blockDataRead[6], 4); // Copy only digits
    trackNumStr[4] = '\0'; // Ensure null termination

    int trackNumber = atoi(trackNumStr);
    Serial.print("🎶 Extracted Track Number: ");
    Serial.println(trackNumber);

    if (trackNumber <= 0 || trackNumber > myMP3.numSdTracks()) {
        Serial.println("❌ Invalid track number.");
        return;
    }

    playTrack(trackNumber);
    rfid.PICC_HaltA();
}


void playTrack(int track) {
  Serial.print("▶️ Playing track: ");
  Serial.println(track);
  myMP3.play(track);
  isPlaying = true;
  digitalWrite(LED_PIN, HIGH); // Turn LED on while playing
}

void checkButton() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); // Debounce
    if (isPlaying) {
      Serial.println("⏸️ Pausing playback...");
      myMP3.pause();
      isPlaying = false;
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.println("▶️ Resuming playback...");
      myMP3.resume();
      isPlaying = true;
      digitalWrite(LED_PIN, HIGH);
    }
  }
}

void checkPlaybackStatus() {
  if (digitalRead(BUSY_PIN) == HIGH && isPlaying) {
    Serial.println("⏹️ Playback finished.");
    isPlaying = false;
    digitalWrite(LED_PIN, LOW);
  }
}

void loop() {
  readRFIDTag();
  checkButton();
  checkPlaybackStatus();
  delay(200);
}
