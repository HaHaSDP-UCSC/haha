/**
 * @file lcddevice.c
 * @brief LCD display device specific code
 * @author August Valera (avalera)
 * @version
 * @date 2017-04-20
 */

#include "atmel_start.h"
#include "utils/hahaUtils.h"
#include "lcddevice.h"

void _lcd_init(void)
{
  // Lines with "CrystalFontz" were added by that datasheet and not part of the Hitachi spec
  gpio_set_pin_level(DISP_E, false);
  // Function set
  _lcd_pin_set(0, 0, 0, 1, 1); // Crystalfontz
  // Function set
  _lcd_pin_set(0, 0, 0, 1, 0);
  _lcd_pin_set(0, DISP_CONF_N, DISP_CONF_F, -1, -1); // Crystalfontz
  // Function set
  _lcd_pin_set(0, 0, 0, 1, 0);
  _lcd_pin_set(0, DISP_CONF_N, DISP_CONF_F, -1, -1);
  // Display ON/OFF control
  _lcd_pin_set(0, 0, 0, 0, 0);
  _lcd_pin_set(0, 1, DISP_CONF_D, DISP_CONF_C, DISP_CONF_B);

  _lcd_clear();
}

void _lcd_clear() {
  // Display clear
  _lcd_pin_set(0, 0, 0, 0, 0);
  _lcd_pin_set(0, 0, 0, 0, 1);
  delay(2); // Crystalfontz requires 1.52ms wait
  // Entry mode set
  _lcd_pin_set(0, 0, 0, 0, 0);
  _lcd_pin_set(0, 0, 1, DISP_CONF_I_D, DISP_CONF_S);
}

void _lcd_pin_write(void)
{
  gpio_set_pin_level(DISP_E, true);
  delay(1);
  gpio_set_pin_level(DISP_E, false);
}

void _lcd_pin_set(int RS, int B7, int B6, int B5, int B4) {
  if(RS >= 0) gpio_set_pin_level(DISP_REG, RS ? true : false);
  if(B7 >= 0) gpio_set_pin_level(DISP_B3, B7 ? true : false);
  if(B6 >= 0) gpio_set_pin_level(DISP_B2, B6 ? true : false);
  if(B5 >= 0) gpio_set_pin_level(DISP_B1, B5 ? true : false);
  if(B4 >= 0) gpio_set_pin_level(DISP_B0, B4 ? true : false);
  _lcd_pin_write();
}

void _lcd_line_next() {
  _lcd_pin_set(0, 1, 1, 0, 0);
  _lcd_pin_set(0, 0, 0, 0, 0);
}

void _lcd_write_char(char c) {
  _lcd_pin_set(1, c & (1 << 7), c & (1 << 6), c & (1 << 5), c & (1 << 4));
  _lcd_pin_set(1, c & (1 << 3), c & (1 << 2), c & (1 << 1), c & (1 << 0));
}

void _lcd_write_string(char* str) {
  for(int i = 0; i < strlen(str); i++)
    _lcd_write_char(str[i]);
}

void _lcd_write_buffer(char** buf) {
  _lcd_clear();
  _lcd_write_str(buf[0]);
  _lcd_write_str(buf[2]);
  _lcd_write_str(buf[1]);
  _lcd_write_str(buf[3]);
}
