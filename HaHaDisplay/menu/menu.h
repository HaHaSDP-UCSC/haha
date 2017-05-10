/**
 * @file menu.c
 * @brief Menu and MenuItems library defines
 * @author August Valera (avalera)
 * @version
 * @date 2017-02-28
 */

// TODO: Implement menus with stack memory instead of malloc()

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

Menu* menu_init(void);
int menu_move(Menu* menu, MenuDirection direct);
int menu_set_lcd(Menu* menu);
int menu_destroy(Menu* this);

MenuItem* menu_item_init(MenuItem* parent, char* value);
int menu_item_set_value(MenuItem* this, char* value);
void menu_item_print_tree(MenuItem* this);
void menu_item_print_tree_helper(MenuItem* this, int level);
MenuItem* menu_item_get_next(MenuItem* this);
MenuItem* menu_item_get_prev(MenuItem* this);
MenuItem* menu_item_get_last(MenuItem* this);
int menu_item_destroy(MenuItem* this);
void* menu_item_on_view_default(Menu* menu);
void* menu_item_on_click_default(Menu* menu);


#endif // _HA_MENU_
