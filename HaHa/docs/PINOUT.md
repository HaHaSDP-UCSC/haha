## Project Pinouts

### Power Out

Pins for power were found on the [Xplained datasheet][Xplained].

| Desc  | Xplained Out | Rail In   |
|-------|--------------|-----------|
| 5.0 V | PWR:3        | LEFT:PWR  |
| 3.3 V | PWR:4        | RIGHT:PWR |
| GND   | PWR:2        | LEFT:GND  |

### Power In

| Desc  | LCD In | LCD Desc |
|-------|--------|----------|
| GND   | LCD:1  | V_SS     |
| 5.0 V | LCD:2  | V_DD     |
| 3.3 V | LCD:3  | V_O      |
| GND   | LCD:5  | R/W      |
| 3.3 V | LCD:15 | LED+     |
| GND   | LCD:16 | LED-     |

### Output to LCD Screen

- Schematic Desc: Labels found on the [base station schematic][Schematic]
- SAM B11 Out: Pins on the actual SAM B11 board
- Xplained Out: Pins on the dev board, conversion from SAM B11 found in the [Xplained datasheet][Xplained].
- In: Pins on LCD display
- LCD Desc: Labels found on LCD datasheet

| Schematic Desc | SAMB11 Out | Xplained Out | LCD In | LCD Desc |
|----------------|------------|--------------|--------|----------|
| DISP_BRIGHT    | 12         | EXT1:15      |        |          |
| DISP_B7        | 13         | EXT1:17      | LCD:14 | DB7      |
| DISP_B6        | 14         | EXT3:7       | LCD:13 | DB6      |
| DISP_B5        | 15         | EXT3:12      | LCD:12 | DB5      |
| DISP_B4        | 16         | EXT3:15      | LCD:11 | DB4      |
| DISP_E         | 17         | EXT3:18      | LCD:6  | E        |
| DISP_RS        | 18         | EXT3:17      | LCD:4  | RS       |

### Input from Buttons

- All physical buttons are active low, the other end connected to GND.
- BTN_DOWN is connected to SW0, which is located on the device

| Desc           | SAMB11 In | Xplained Out |
|----------------|-----------|--------------|
| BTN_UP         | 22        | N/A          |
| BTN_UP_TEMP    | 6         | EXT3:13      |
| BTN_DOWN       | 4         | CONFLICT     |
| BTN_DOWN_TEMP  | 9         | EXT1:12      |
| BTN_LEFT       | 3         | CONFLICT     |
| BTN_LEFT_TEMP  | 19        | EXT3:16      |
| BTN_RIGHT      | 23        | N/A          |
| BTN_RIGHT_TEMP | 20        | EXT3:5       |

### UART Out (Target IO)

| Desc     | SAMB11 Out |
|----------|------------|
| UART_CTS | 8          |
| UART_RTS | 7          |
| UART_RX  | 11         |
| UART_TX  | 10         |

### Input Redirect

Pins 3 and 4

[Schematic]:
[Xplained]: http://www.atmel.com/Images/Atmel-42664-ATSAMB11-Xplained-Pro_UserGuide.pdf
