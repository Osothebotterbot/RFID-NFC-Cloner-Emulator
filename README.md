# RFID/NFC Cloner & Emulator — Pro Edition

An industrial-grade Arduino solution to read, clone, emulate and log MIFARE Classic & NTAG2xx cards.  
Adds serial menu, SD-card logging, buzzer alerts, multi-key support and timestamped dumps.

---

## 1. New Features

• **Serial Menu & Commands**  
  – ‘R’: Read & dump card to RAM & SD as CSV  
  – ‘W’: Write last dump to blank tag  
  – ‘L’: Load last dump from SD into RAM  
  – ‘E’: Enter Emulation mode (act as card)  
  – ‘K’: Cycle Authentication Key (A/B/default/custom)  
  – ‘C’: Clear RAM dump  
  – ‘?’: Show menu

• **SD Card Logging**  
  – Saves each dump as `UID_YYYYMMDD_HHMMSS.csv`  
  – CSV: `sector,block,hex0…hex15`

• **Timestamping**  
  – Uses DS3231 RTC to timestamp file names & serial output.

• **Multi-Key Support**  
  – Default keys A/B, plus user-programmable keys via Serial.

• **Audible & Visual Feedback**  
  – Bidirectional LED blink + buzzer beep on success/failure.

---

## 2. Bill of Materials (Additions)

| Qty | Component                     | Notes                              |
|-----|-------------------------------|------------------------------------|
| +1  | microSD card module           | for CSV logging                    |
| +1  | DS3231 RTC module             | for accurate timestamps            |
| +1  | Piezo buzzer                  | audible alerts                     |
| +1  | 1 kΩ trim-pot                 | optional custom key entry via ADC  |
|     | all other components same     |                                    |

---

## 3. Wiring Overview

        Arduino        PN532 SPI        DS3231 RTC      microSD module      Push Btn   LED + Buzzer
        ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
        │ 5V ─► VCC    ─► VCC        SDA ─► A4       MOSI ─► D11      BUTTON ─► D3 (PULLUP)     LED ─► D6 │
        │ GND─► GND    ─► GND        SCL ─► A5       MISO ─► D12      to GND              BZR ─► D7 │
        │ D10─► SS     ─► SS         SQW ─► D2       SCK  ─► D13                          220Ω┴ GND │
        │ D11─► MOSI   ─► MOSI       INT ─► D9       CS   ─► D4                                   │
        │ D12─► MISO   ─► MISO       RST ─► D8       VCC  ─► 5V                                   │
        │ D13─► SCK    ─► SCK                          GND  ─► GND                                  │
        │                                                                                         │
        │ A0─► POT (optional for custom key entry)                                               │
        └────────────────────────────────────────────────────────────────────────────────────────────┘

4. Operation Flow

       Power Up • RTC sync; SD mount; PN532 init.

       Serial Menu (?): choose operation.

       Read (R) • Presents card → dumps sectors/blocks into RAM & UID_TIMESTAMP.csv.

       Write (W) • Presents blank tag → writes RAM dump.

       Load (L) • Reload dump from last SD file into RAM.

       Emulation (E) • PN532 emulates UID + data from RAM.

       Key Select (K) • Cycle through default Key A/B or custom (via POT/Serial).

       Clear (C) • Erase RAM dump before new read.

       LED+Beep: success = two short blinks/beeps; fail = long blink/beep.

       Proceed to the enhanced Arduino sketch below.

