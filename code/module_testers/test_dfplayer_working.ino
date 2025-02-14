#include <Arduino.h>
#include <DFPlayerMini_Fast.h>
#include <HardwareSerial.h>

// Define ESP32-C6 TX/RX pins for DFPlayer Mini
#define DFPLAYER_RX 17  // ESP32-C6 RX (DFPlayer TX)
#define DFPLAYER_TX 16  // ESP32-C6 TX (DFPlayer RX)
#define BUSY_PIN 15     // Optional: DFPlayer BUSY pin to check playback status

// Create a HardwareSerial instance on UART1
HardwareSerial mySerial(1);
DFPlayerMini_Fast myMP3;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing DFPlayer Mini...");

  mySerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);

  // Initialize DFPlayer
  if (!myMP3.begin(mySerial, true)) {  
    Serial.println("❌ DFPlayer initialization failed! Check wiring.");
    return;
  }

  Serial.println("✅ DFPlayer Initialized!");
  
  delay(2000);  // Allow time for DFPlayer to detect SD card

  // Check total MP3 tracks on SD card
  int totalTracks = myMP3.numSdTracks();
  Serial.print("🎵 Total MP3 files detected: ");
  Serial.println(totalTracks);

  if (totalTracks > 0) {
    Serial.println("▶️ Playing first track...");
    myMP3.play(2);  // Play track 0001.mp3
  } else {
    Serial.println("❌ No tracks found on SD card.");
  }

  pinMode(BUSY_PIN, INPUT);
}

void loop() {
  if (digitalRead(BUSY_PIN) == LOW) {
    Serial.println("🎵 DFPlayer is playing audio.");
  } else {
    //Serial.println("⏹️ DFPlayer is idle.");
  }
  
  delay(2000);
}
