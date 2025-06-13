// File: RFID_Cloner.ino
// Author: Bocaletto Luca
// Read, clone & emulate MIFARE Classic 1K / NTAG2xx cards via PN532
// Requires Adafruit_PN532 library

        #include <SPI.h>
        #include <SD.h>
        #include <Wire.h>
        #include <RTClib.h>
        #include <Adafruit_PN532.h>

        //=== Pin Definitions ===
        #define PN532_SS     10
        #define PN532_MOSI   11
        #define PN532_MISO   12
        #define PN532_SCK    13
        #define PN532_IRQ    9
        #define PN532_RST    8

        #define SD_CS        4
        #define RTC_SQW      2

        #define BUTTON_PIN   3
        #define LED_PIN      6
        #define BUZZER_PIN   7
        #define POT_PIN      A0    // optional custom key

        //=== Global Instances ===
        Adafruit_PN532 nfc(PN532_SS);
        RTC_DS3231       rtc;

        struct CardDump {
            uint8_t uid[7], uidLen;
            uint8_t data[16][4][16];
            bool    valid;
        } dump;

        const uint8_t defaultKeyA[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        const uint8_t defaultKeyB[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        uint8_t *activeKey = (uint8_t*)defaultKeyA;

        //=== Utility Prototypes ===
        void menu();
        void readCard();
        void writeCard();
        void emuCard();
        void loadDump();
        void saveDump();
        void beep(int ms);
        String timestamp();

        void setup() {
            Serial.begin(115200);
            pinMode(BUTTON_PIN, INPUT_PULLUP);
            pinMode(LED_PIN, OUTPUT);
            pinMode(BUZZER_PIN, OUTPUT);
            SPI.begin();

            // RTC
            if (!rtc.begin()) { while(1); }
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

            // SD
            if (!SD.begin(SD_CS)) { while(1); }

            // NFC
            nfc.begin();
            if (!nfc.getFirmwareVersion()) { while(1); }
            nfc.SAMConfig();

            dump.valid = false;
            menu();
        }

        void loop() {
            if (Serial.available()) {
                char cmd = toupper(Serial.read());
                switch (cmd) {
                    case 'R': readCard();      break;
                    case 'W': writeCard();     break;
                    case 'E': emuCard();       break;
                    case 'L': loadDump();      break;
                    case 'S': saveDump();      break;
                    case 'K': activeKey = (activeKey==defaultKeyA?defaultKeyB:defaultKeyA);
                             Serial.println("Key toggled"); beep(100); break;
                    case 'C': dump.valid=false; Serial.println("Cleared RAM"); beep(50); break;
                    case '?': menu();          break;
                }
            }
        }

        void menu() {
            Serial.println(F("\n=== RFID/NFC Cloner Pro ==="));
            Serial.println(F("R - Read & dump card"));
            Serial.println(F("W - Write RAM dump to tag"));
            Serial.println(F("E - Emulate dumped card"));
            Serial.println(F("L - Load dump from SD"));
            Serial.println(F("S - Save dump to SD"));
            Serial.println(F("K - Toggle Key A/B"));
            Serial.println(F("C - Clear RAM dump"));
            Serial.println(F("? - Show this menu\n"));
        }

        void readCard() {
            Serial.println("Present tag to read...");
            while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, dump.uid, &dump.uidLen)) {}
            Serial.print("UID: ");
            for (uint8_t i=0;i<dump.uidLen;i++) {
                Serial.print(dump.uid[i], HEX); Serial.print(':');
            }
            Serial.println();
            for (uint8_t s=0;s<16;s++) {
                if (!nfc.mifareclassic_AuthenticateSector(dump.uid, dump.uidLen, s, 0, activeKey)) {
                    Serial.print("Auth fail sector ");Serial.println(s);
                    continue;
                }
                for (uint8_t b=0;b<4;b++) {
                    nfc.mifareclassic_ReadDataBlock(s*4+b, dump.data[s][b]);
                }
            }
            dump.valid = true;
            Serial.println("Read complete"); beep(100);
        }

        void writeCard() {
            if (!dump.valid) { Serial.println("No dump in RAM"); return; }
            Serial.println("Present blank tag to write...");
            uint8_t uid2[7], len2;
            while (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid2, &len2)) {}
            for (uint8_t s=0;s<16;s++) {
                if (!nfc.mifareclassic_AuthenticateSector(uid2,len2,s,0,activeKey)) continue;
                for (uint8_t b=0;b<4;b++) {
                    nfc.mifareclassic_WriteDataBlock(s*4+b, dump.data[s][b]);
                }
            }
            Serial.println("Write complete"); beep(200);
        }

        void emuCard() {
            if (!dump.valid) { Serial.println("No dump in RAM"); return; }
            Serial.println("Entering emulation...");
            nfc.inListPassiveTarget();  // start emulation
            Serial.println("Emulating. Reset to exit."); beep(150);
            while (1);
        }

        void saveDump() {
            if (!dump.valid) { Serial.println("No dump in RAM"); return; }
            String fn = String(dump.uid[0], HEX);
            fn += "_" + timestamp() + ".csv";
            File f = SD.open(fn, FILE_WRITE);
            for (uint8_t s=0;s<16;s++) {
                for (uint8_t b=0;b<4;b++) {
                    f.print(s); f.print(','); f.print(b);
                    for (uint8_t i=0;i<16;i++) {
                        f.print(','); f.print(dump.data[s][b][i], HEX);
                    }
                    f.println();
                }
            }
            f.close();
            Serial.print("Saved to "); Serial.println(fn); beep(100);
        }

        void loadDump() {
            Serial.println("Enter filename to load:");
            while (!Serial.available()) {}
            String fn = Serial.readStringUntil('\n');
            File f = SD.open(fn.c_str());
            if (!f) { Serial.println("File not found"); return; }
            while (f.available()) {
                uint8_t s = f.parseInt();
                uint8_t b = f.parseInt();
                for (uint8_t i=0;i<16;i++) {
                    dump.data[s][b][i] = f.parseInt();
                }
            }
            f.close();
            dump.valid = true;
            Serial.println("Load complete"); beep(100);
        }

        void beep(int ms) {
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(BUZZER_PIN, HIGH);
            delay(ms);
            digitalWrite(LED_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
        }

        String timestamp() {
            DateTime t = rtc.now();
            char buf[20];
            snprintf(buf, 20, "%04u%02u%02u_%02u%02u%02u",
                t.year(), t.month(), t.day(), t.hour(), t.minute(), t.second());
            return String(buf);
        }
