/**
 * @file lcd.h
 * @brief LCD display general code
 * @author August Valera (avalera)
 * @version
 * @date 2017-04-20
 */


#ifndef _HA_LCD_H_
#define _HA_LCD_H_

#include "lcddevice.h"
#include "utils/hahaUtils.h"

char lcd_buffer[LCD_ROWS][LCD_COLS + 1]; // Extra space for null terminator

#endif /* _HA_LCD_H_ */
