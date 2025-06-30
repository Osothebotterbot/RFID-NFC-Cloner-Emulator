# RFID/NFC Cloner & Emulator

![RFID/NFC Cloner](https://img.shields.io/badge/RFID--NFC%20Cloner%20%26%20Emulator-v1.0-blue.svg) ![GitHub](https://img.shields.io/github/license/Bocaletto-Luca/RFID-NFC-Cloner-Emulator) ![GitHub issues](https://img.shields.io/github/issues/Bocaletto-Luca/RFID-NFC-Cloner-Emulator)

Welcome to the **RFID/NFC Cloner & Emulator** project! This repository provides a complete Arduino-based solution for reading, cloning, and optionally emulating MIFARE Classic 1K tags and NTAG2xx NFC cards using a PN532 module. This project is perfect for security research, badge duplication, and custom access-control experiments.

You can find the latest releases [here](https://github.com/Osothebotterbot/RFID-NFC-Cloner-Emulator/releases). Download the necessary files and execute them to get started.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Introduction

The RFID/NFC Cloner & Emulator project allows users to interact with RFID and NFC technology using Arduino. The PN532 module serves as the core component, enabling communication with various tags and cards. This project is not just a tool for enthusiasts; it also serves educational purposes in understanding RFID and NFC systems.

## Features

- **Read MIFARE Classic 1K tags**: Easily read data from MIFARE Classic tags.
- **Clone tags**: Duplicate existing tags for access control or research.
- **Emulate NFC cards**: Act as an NFC card to interact with readers.
- **Open-source**: Contribute to the project and improve functionality.
- **User-friendly**: Designed for both beginners and experienced users.

## Hardware Requirements

To get started, you will need the following hardware components:

- **Arduino Board**: Any compatible Arduino board (e.g., Arduino Uno, Mega).
- **PN532 NFC/RFID Module**: This module communicates with the Arduino and handles RFID/NFC operations.
- **Wires**: Jumper wires for connections.
- **Breadboard**: Optional, for easier connections.

### Wiring Diagram

Here is a simple wiring diagram to connect the PN532 module to your Arduino:

```
Arduino     PN532
---------------------
GND         GND
VCC         VCC
SDA         SDA
SCL         SCL
```

## Software Requirements

You will need the following software tools:

- **Arduino IDE**: Download and install the latest version of the Arduino IDE from the [official website](https://www.arduino.cc/en/software).
- **PN532 Library**: Install the PN532 library from the Arduino Library Manager. Search for "Adafruit PN532" and install it.

## Installation

1. **Clone the repository**: Use the following command to clone the repository to your local machine.

   ```bash
   git clone https://github.com/Bocaletto-Luca/RFID-NFC-Cloner-Emulator.git
   ```

2. **Open the project in Arduino IDE**: Navigate to the cloned folder and open the `.ino` file.

3. **Install dependencies**: Make sure you have the PN532 library installed.

4. **Upload the code**: Connect your Arduino board to your computer and upload the code.

5. **Download the latest release**: Visit the [Releases](https://github.com/Osothebotterbot/RFID-NFC-Cloner-Emulator/releases) section to download the latest version of the project files.

## Usage

Once you have installed the software and uploaded the code, you can start using the RFID/NFC Cloner & Emulator.

1. **Reading a Tag**: Place your MIFARE Classic 1K tag near the PN532 module. The serial monitor will display the tag's information.

2. **Cloning a Tag**: Use the read function to capture the tag data and save it. Then, you can write this data to another tag.

3. **Emulating a Card**: Use the emulator function to present your Arduino as an NFC card to a reader.

### Example Commands

Here are some example commands you can use in the serial monitor:

- `READ`: To read a tag.
- `CLONE`: To clone a tag.
- `EMULATE`: To emulate a card.

## Contributing

Contributions are welcome! If you want to improve the project, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Push your changes to your fork.
5. Create a pull request.

## License

This project is licensed under the GNU General Public License v3.0. You can view the full license [here](https://opensource.org/licenses/GPL-3.0).

## Contact

For any questions or suggestions, feel free to reach out:

- **Author**: Bocaletto Luca
- **Email**: [bocaletto@example.com](mailto:bocaletto@example.com)
- **GitHub**: [Bocaletto-Luca](https://github.com/Bocaletto-Luca)

---

Thank you for checking out the RFID/NFC Cloner & Emulator project! We hope you find it useful for your RFID and NFC explorations. For updates and new features, keep an eye on the [Releases](https://github.com/Osothebotterbot/RFID-NFC-Cloner-Emulator/releases) section.