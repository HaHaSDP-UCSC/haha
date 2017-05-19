/**
 * @file menu.c
 * @brief Menu and MenuItems library defines
 * @author August Valera (avalera)
 * @version
 * @date 2017-02-28
 */

#ifndef _HA_MENU_
#define _HA_MENU_

#include "utils/hahaUtils.h"
#include "lcd/lcd.h"

#define MENU_MAXLEN 32
#define MENU_WRAP true

#define MENU_CHAR_SELECTED '>'
#define MENU_CHAR_SELECTABLE 126

typedef struct MenuItem MenuItem;
typedef struct MenuItem {
  void* (*onView)();
  void* (*onClick)();
  char value[MENU_MAXLEN];
  bool active;
  MenuItem* parent;
  MenuItem* child;
  MenuItem* prev;
  MenuItem* next;
} MenuItem;

typedef struct Menu {
  MenuItem* root;
  MenuItem* current;
  void* onViewRet;
  void* onClickRet;
  char** inputBuffer;
  char** outputBuffer;
} Menu;

typedef enum MenuDirection {
  MENU_UNDEF = 0,
  MENU_UP,
  MENU_DOWN,
  MENU_LEFT,
  MENU_RIGHT
} MenuDirection;

/**
 * @brief Constructor for global menu
 * @return New Menu
 */
Menu* menu_init(void);

/**
 * @brief Moves menu cursor in given direction
 * @param menu Menu object containing cursor
 * @param direct Direction to move
 * @return 0 on success, 1 on invalid direction, -1 on error
 */
int menu_move(Menu* menu, MenuDirection direct);

/**
 * @brief Sets LCD based on values of menu items, default printer
 * @param menu Menu
 * @return 0 on success, else error
 */
int menu_set_lcd(Menu* menu);

/**
 * @brief Destructor for global menu
 * @param this Menu to destroy
 * @return 0 on success, else error
 */
int menu_destroy(Menu* this);

/**
 * @brief Constructor for menu item, inserted in list of parent
 * @param parent
 * @return New MenuItem
 */
MenuItem* menu_item_init(MenuItem* parent, char* value);

/**
 * @brief Sets value of MenuItem
 * @param this MenuItem to edit
 * @param value String value to copy over
 * @return 0 on success, else error
 */
int menu_item_set_value(MenuItem* this, char* value);

MenuItem* menu_item_get_next(MenuItem* this);
MenuItem* menu_item_get_prev(MenuItem* this);
MenuItem* menu_item_get_last(MenuItem* this);

/**
 * @brief Destructor for MenuItem, recursive on children
 * @param this MenuItem to destroy
 * @return 0 on success, else error
 */
int menu_item_destroy(MenuItem* this);

#endif // _HA_MENU_
