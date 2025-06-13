# RFID/NFC Cloner & Emulator

A turnkey Arduino project to read, clone and (optionally) emulate MIFARE Classic 1K tags or NTAG2xx NFC cards using a PN532 module. Perfect for security research, badge duplication or custom access‐control experiments.

**Files**  
- `README.md`  
- `RFID_Cloner.ino`

---

## 1. Concept

1. **Read Mode**  
   - Scan a MIFARE Classic or NTAG2xx card.  
   - Dump its UID and all data blocks to RAM and Serial.  
2. **Write Mode**  
   - Present a blank (or rewritable) tag.  
   - Write back the dumped data, recreating an exact clone.  
3. **Emulation Mode (Optional)**  
   - Put the PN532 into “card emulation” so it behaves as the cloned tag.  
   - Any standard RFID reader will treat your Arduino as the original badge.  

LED and Serial feedback guide you through each step.

---

## 2. Bill of Materials

| Qty | Component                 | Notes                         |
|-----|---------------------------|-------------------------------|
| 1   | Arduino Uno / Nano        | 5 V version                   |
| 1   | PN532 NFC RFID module     | SPI interface (Adafruit/Seeed)|
| 2+  | MIFARE Classic 1K cards   | “Victim” and blank clone tag  |
| 1   | LED (5 mm) + 220 Ω resistor| Status indicator              |
| 1   | Push-button               | Mode selector (Read/Write)    |
|     | Jumper wires & breadboard |                               |
|     | USB cable                 | Programming & power           |

---

## 3. Wiring Diagram

```text
  Arduino Uno        PN532 (SPI Mode)      Push-Button       LED
  ┌─────────────────────────────────────────────────────────────────┐
  │   5 V     ──► VCC                                              │
  │   GND     ──► GND                                             │
  │   D10     ──► SS (SSEL)                                       │
  │   D11     ──► MOSI                                           │
  │   D12     ──► MISO                                           │
  │   D13     ──► SCK                                            │
  │   D9      ──► RSTO (reset)                                   │
  │   D2      ──► IRQ (interrupt)                                │
  │                                                             │
  │   D3   ◄── BUTTON ── to GND (use INPUT_PULLUP)               │
  │                                                             │
  │   D6     ──► LED ──220 Ω──┴ GND                              │
  └─────────────────────────────────────────────────────────────┘

