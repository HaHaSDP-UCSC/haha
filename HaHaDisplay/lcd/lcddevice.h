/**
 * @file lcddevice.h
 * @brief LCD display device specific code
 * @author August Valera (avalera)
 * @version
 * @date 2017-04-20
 */

#ifndef _HA_LCDDEVICE_H_
#define _HA_LCDDEVICE_H_

#include "utils/hahaUtils.h"

// Required info for lcd module

#define LCD_COLS 16
#define LCD_ROWS 4

// Device specific configuration

#define DISP_CONF_N 1 // Line number (0: 1 line display, 1: 2 line display)
#define DISP_CONF_F 0 // Font type (0: 5x8 dots, 1: 5x11 dots)

#define DISP_CONF_D 1 // Display ON/OFF (when off, data still stored in DDRAM)
#define DISP_CONF_C 1 // Cursor ON/OFF
#define DISP_CONF_B 1 // Cursor blink ON/OFF

#define DISP_CONF_I_D 1 // Cursor increment/decrement (1: increment)
#define DISP_CONF_S 0 // Shift (0: no shift)


/**
 * @brief initializes the LCD display
 */
void _lcd_init(void);


/**
 * @brief clears the LCD display
 */
void _lcd_clear();

/**
 * @brief toggles LCD E pin to write current register contents to LCD
 */
void _lcd_pin_write(void);

/**
 * @brief sets register contents, 1: high, 0: low, -1: no change
 *
 * @param RS register
 * @param B7 register
 * @param B6 register
 * @param B5 register
 * @param B4 register
 */
void _lcd_pin_set(int RS, int B7, int B6, int B5, int B4);

/**
 * @brief switches display cursor to next line
 */
void _lcd_line_next();

/**
 * @brief writes character to the display
 *
 * @param c character
 */
void _lcd_write_char(char c);

/**
 * @brief writes string to the display
 *
 * @param str string
 */
void _lcd_write_string(char str[]);

void _lcd_write_string_pad(char str[]);

/**
 * @brief writes entire display buffer
 *
 * @param buf buffer
 */
void _lcd_update(char buf[][LCD_COLS + 1]);

#endif /* _HA_LCDDEVICE_H_ */
