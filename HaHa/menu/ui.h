/**
 * @file ui.h
 * @brief User interface
 * @author August Valera (avalera)
 * @version
 * @date 2017-03-07
 */

#ifndef _HA_UI_
#define _HA_UI_

#include "utils/hahaUtils.h"
#include "menu.h"
#include "atmel_start.h"
#include <string.h>
// #include "base.h"

#define UI_INPUT_ENTER 126

// Global variables
Menu *menu;
MenuItem* ui_item_helpreq;
MenuItem* ui_item_helpdeny;
MenuItem* ui_item_helpresp;

/**
 * @brief initializes the user interface
 */
void ui_init(void);

/**
 * @brief moves current menu item in a direction
 * @param direct direction to move, one of {LEFT, RIGHT, UP, DOWN}
 */
void ui_move(MenuDirection direct);

/**
 * @brief sets the lcd based on the values of the MenuItems, basic printer used by onview_default
 * @return 
 */
int ui_set_lcd(void);

/**
 * @brief checks for button input, and calls ui_move accordingly
 */
void ui_update(void);

/**
 * @brief inits a menu item
 * @param parent parent menu item in menu tree
 * @param value string value, usually printed out
 * @return new menu item
 */
MenuItem* ui_item_init(MenuItem* parent, char* value);

/**
 * @brief default onview method, calls ui_set_lcd, only prints out values of fellow children
 * @param menu menu tree
 * @return NULL
 */
void* ui_item_default_onview(Menu* menu);

/**
 * @brief default onclick method, sets current to current's first child if exists
 * @param menu menu tree
 * @return NULL
 */
void* ui_item_default_onclick(Menu* menu);

/**
 * @brief custom onview for user info, displays user info in full screen with prompts to edit
 * @param menu menu tree
 * @return NULL
 */
void* ui_userinfo_onview(Menu* menu);

/**
 * @brief custom onclick for user info, starts user input editing current item
 * @param menu menu tree
 * @return NULL
 */
void* ui_userinfo_onclick(Menu* menu);

/**
 * @brief initializes user input state by creating children for letters
 * @param menu menu tree
 * @param value string to display on first line of LCD, give user context on what they're editing
 * @param letters character set for creating string, such as alpha, alphacase (a and A), alphanum, num
 * @param output string to copy output to on save
 */
void ui_input_init(Menu* menu, char* value, char* letters, char** output);

/**
 * @brief save user input to output specified in input_init, then input_destroy
 * @param menu menu tree
 */
void ui_input_save(Menu* menu);

/**
 * @brief delete a character from the input buffer
 * @param menu menu tree
 */
void ui_input_delete(Menu* menu);

/**
 * @brief destroy the input tree, going back to parent
 * @param menu menu tree
 */
void ui_input_destroy(Menu* menu);

/**
 * @brief onview method on user input, displays current input buffer and selected character
 * @param menu menu tree
 */
void* ui_input_onview(Menu* menu);

/**
 * @brief onclick method on user input, appends selected character to input buffer (or saves, if ->)
 * @param menu menu tree
 */
void* ui_input_onclick(Menu* menu);

/**
 * @brief onclick method on contact list, regenerates contact list
 * @param menu menu tree
 */
void* ui_contacts_onclick(Menu* menu);

/**
 * @brief onclick method on help request, sends help request
 * @param menu menu tree
 */
void ui_helpreq_onclick(Menu *menu);

/**
 * @brief onview method triggered on help request deny, sends deny and then transitions to root
 * @param menu menu tree
 */
void* ui_helpdeny_onview(Menu* menu);

/**
 * @brief onview method on help request received, prompts user to accept or deny
 * @param menu menu tree
 */
void* ui_helpresp_onview(Menu* menu);

/**
 * @brief onclick method triggered help request accept, sends accept and then transitions to root
 * @param menu menu tree
 */
void* ui_helpresp_onclick(Menu* menu);

#endif // _HA_INIT_
