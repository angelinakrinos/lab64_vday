#include <SPI.h>

// Define SPI pins for ESP32-C6
#define SCK_PIN   19
#define MISO_PIN  20
#define MOSI_PIN  18
#define SS_PIN    2   // Change if needed

SPIClass spi = SPIClass(FSPI);  // ESP32-C6 uses FSPI


void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Testing SPI Bus...");
  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  digitalWrite(SS_PIN, LOW);
  delay(10);
  
  uint8_t response = spi.transfer(0x00);  // Send dummy data
  digitalWrite(SS_PIN, HIGH);

  Serial.print("SPI Transfer Response: 0x");
  Serial.println(response, HEX);

  if (response == 0x00 || response == 0xFF) {
    Serial.println("❌ No response from device - Check wiring & SPI pins.");
  } else {
    Serial.println("✅ SPI communication looks OK!");
  }
}

void loop() {
  // Nothing here
}
