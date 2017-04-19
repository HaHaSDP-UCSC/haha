## Project Pinouts

### Power Out

| Desc  | Xplained Out | Rail In   |
| ---   | ---          | ---       |
| 5.0 V | PWR:3        | LEFT:PWR  |
| 3.3 V | PWR:4        | RIGHT:PWR |
| GND   | PWR:2        | LEFT:GND  |

### Power In

| Desc  | LCD Desc |
| ---   | ---      |
| GND   | V_SS     |
| 5.0 V | V_DD     |
| GND   | LED-     |
| 3.3 V | LED+     |

### Output to LCD Screen

- Schematic Desc: Labels found on the base station schematic
- SAM B11 Out: Pins on the actual SAM B11 board
- Xplained Out: Pins on the dev board, conversion from SAM B11 found in the [Xplained datasheet](Xplained).
- In: Pins on LCD display
- LCD Desc: Labels found on LCD datasheet

| Schematic Desc | SAM B11 Out | Xplained Out | In     | LCD Desc |
| ---            | ---         | ---          | ---    | ---      |
| DISP_BRIGHT    | 13          | EXT1:17      |        |          |
| DISP_B3        | 14          | EXT3:11      | LCD:14 | DB7      |
| DISP_B2        | 15          | EXT3:12      | LCD:13 | DB6      |
| DISP_B1        | 16          | EXT3:15      | LCD:12 | DB5      |
| DISP_B0        | 17          | EXT3:18      | LCD:11 | DB4      |
| DISP_E         | 18          | EXT3:17      | LCD:6  | E        |
| DISP_REG       | 19          | EXT3:16      | LCD:4  | RS       |
| DISP_CTRST     | 20          | EXT3:5       |        |          |

[Schematic]()
[Xplained](www.atmel.com/Images/Atmel-42664-ATSAMB11-Xplained-Pro_UserGuide.pdf)