/**
 * @file init.h
 * @brief User interface
 * @author August Valera (avalera)
 * @version
 * @date 2017-03-07
 */

#ifndef _HA_INIT_
#define _HA_INIT_

#include "utils/hahaUtils.h"
#include "menu.h"
#include "atmel_start.h"
#include <string.h>
// #include "base.h"

#define UI_INPUT_ENTER 126

// Global variables
Menu *menu;

void ui_init(void);
void ui_move(MenuDirection direct);
int ui_set_lcd(void);
void ui_update(void);
MenuItem* ui_item_init(MenuItem* parent, char* value);
void* ui_item_onview_default(Menu* menu);
void* ui_item_onclick_default(Menu* menu);

void* ui_onview_userinfo(Menu* menu);
void* ui_onclick_userinfo(Menu* menu);

void ui_input_init(Menu* menu, char* value, char* letters, char** output);
void ui_input_save(Menu* menu);
void ui_input_delete(Menu* menu);
void ui_input_destroy(Menu* menu);
void* ui_input_onview(Menu* menu);
void* ui_input_onclick(Menu* menu);

#endif // _HA_INIT_
