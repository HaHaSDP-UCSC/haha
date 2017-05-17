/**
 * @file menu.c
 * @brief Menu and MenuItems library defines
 * @author August Valera (avalera)
 * @version
 * @date 2017-02-28
 */

#ifndef _HA_MENU_
#define _HA_MENU_

#include "utils/hahalib.h"
#include "lcd/lcd.h"

#define MENU_MAXLEN 32
#define MENU_WRAP true

#define MENU_CHAR_SEL_CHLD '>'
#define MENU_CHAR_CHLD 126
#define MENU_CHAR_SEL_FUNC '>'
#define MENU_CHAR_FUNC 126

typedef struct MenuItem MenuItem;
typedef struct MenuItem {
  void (*onView)();
  void (*onClick)();
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

/**
 * @brief Prints out tree representation of MenuItem and children
 * @param this Root of tree
 */
void menu_item_print_tree(MenuItem* this);

/**
 * @brief Recursive helper of menuItemPrintTree()
 * @param this Root of tree
 * @param level Level of recursion we are in
 */
void menu_item_print_tree_helper(MenuItem* this, int level);

MenuItem* menu_item_get_next(MenuItem* this);
MenuItem* menu_item_get_prev(MenuItem* this);
MenuItem* menu_item_get_last(MenuItem* this);

/**
 * @brief Destructor for MenuItem, recursive on children
 * @param this MenuItem to destroy
 * @return 0 on success, else error
 */
int menu_item_destroy(MenuItem* this);
void* menu_item_on_view_default(Menu* menu);
void* menu_item_on_click_default(Menu* menu);

#endif // _HA_MENU_