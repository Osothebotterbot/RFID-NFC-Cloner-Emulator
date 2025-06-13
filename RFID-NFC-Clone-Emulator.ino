// File: RFID_Cloner.ino
// Author: Bocaletto Luca
// Read, clone & emulate MIFARE Classic 1K / NTAG2xx cards via PN532
// Requires Adafruit_PN532 library

#include <SPI.h>
#include <Adafruit_PN532.h>

//=== Pin Configuration ===
#define PN532_SS    10   // SPI SS
#define PN532_MOSI  11   // SPI MOSI
#define PN532_MISO  12   // SPI MISO
#define PN532_SCK   13   // SPI SCK
#define PN532_IRQ   2    // IRQ
#define PN532_RSTO  9    // RSTO

#define BUTTON_PIN  3    // Mode select (READ/WRITE)
#define LED_PIN     6    // Status LED

//=== PN532 Initialization ===
Adafruit_PN532 nfc(PN532_SS);

//=== Storage for up to 16 sectors × 4 blocks/sector × 16 bytes/block ===
uint8_t blockData[16][4][16];
uint8_t uid[7], uidLen;

bool writeMode = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  while (!Serial);

  SPI.begin();
  nfc.begin();
  uint32_t version = nfc.getFirmwareVersion();
  if (!version) {
    Serial.println("PN532 not found!");
    while (1);
  }
  nfc.SAMConfig();
  Serial.println("PN532 Ready. Waiting for card...");
}

void loop() {
  // Button toggles Read/Write mode
  if (digitalRead(BUTTON_PIN) == LOW) {
    writeMode = !writeMode;
    Serial.print(writeMode ? ">> WRITE MODE <<" : ">> READ MODE <<");
    Serial.println("  (press again to toggle)");
    delay(500);
  }

  // Poll for a card
  if (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen)) {
    return;
  }
  Serial.print("Card detected, UID: ");
  for (uint8_t i = 0; i < uidLen; i++) {
    Serial.print(uid[i], HEX);
    if (i < uidLen - 1) Serial.print(":");
  }
  Serial.println();

  // Use default key A for all sectors: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
  uint8_t keyA[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

  if (!writeMode) {
    // READ MODE: dump all 16 sectors
    for (uint8_t sector = 0; sector < 16; sector++) {
      if (!nfc.mifareclassic_AuthenticateSector(uid, uidLen, sector, 0, keyA)) {
        Serial.print("Auth failed on sector "); Serial.println(sector);
        continue;
      }
      for (uint8_t block = 0; block < 4; block++) {
        uint8_t data[16];
        if (nfc.mifareclassic_ReadDataBlock(sector * 4 + block, data)) {
          memcpy(blockData[sector][block], data, 16);
          Serial.print("Sector "); Serial.print(sector);
          Serial.print(" Block "); Serial.print(block);
          Serial.print(": ");
          for (uint8_t i = 0; i < 16; i++) {
            Serial.print(data[i], HEX);
            if (i < 15) Serial.print(" ");
          }
          Serial.println();
        }
      }
    }
    Serial.println("Dump complete. Press button to switch to WRITE MODE.");
  } else {
    // WRITE MODE: write stored dump back to blank card
    for (uint8_t sector = 0; sector < 16; sector++) {
      if (!nfc.mifareclassic_AuthenticateSector(uid, uidLen, sector, 0, keyA)) {
        Serial.print("Auth failed on sector "); Serial.println(sector);
        continue;
      }
      for (uint8_t block = 0; block < 4; block++) {
        uint8_t *data = blockData[sector][block];
        if (nfc.mifareclassic_WriteDataBlock(sector * 4 + block, data)) {
          Serial.print("Wrote sector "); Serial.print(sector);
          Serial.print(" block "); Serial.println(block);
        } else {
          Serial.print("Write failed sector "); Serial.print(sector);
          Serial.print(" block "); Serial.println(block);
        }
      }
    }
    Serial.println("Write complete. You may now use the cloned card.");
  }

  // LED blink to show success
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(2000);
}
