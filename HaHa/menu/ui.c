/**
 * @file init.c
 * @brief User interface
 * @author August Valera (avalera)
 * @version
 * @date 2017-03-07
 */

#include "ui.h"

Menu* ui_init(void) {
  MenuItem* temp;
  Menu* menu = menu_init();
  MenuItem* root = menu->root;
  MenuItem* mm = menu_item_init(root, "Main Menu");
  MenuItem* contacts = menu_item_init(mm, "Contact list");
  MenuItem* user = menu_item_init(mm, "User info");
  temp = menu_item_init(user, "__USERPORT__");
  temp = menu_item_init(user, "__USERNAME__");
  temp = menu_item_init(user, "__USERADDR__");
  temp = menu_item_init(user, "__USERCALL__");
  MenuItem* set = menu_item_init(mm, "Device settings");
  menu_item_init(set, "Office mode");
  MenuItem* setAlert = menu_item_init(set, "Alert settings");
  menu_item_init(setAlert, "Sound");
  menu_item_init(setAlert, "Lights");
  MenuItem* setTest = menu_item_init(set, "Test button");
  menu_item_init(setTest, "BUTTON TEST");
  menu_item_init(setTest, "Press button now");
  menu_item_init(setTest, "to test signal.");
  MenuItem* setTestStop = menu_item_init(setTest, "Stop testing");
  setTestStop->onClick = jumpToRoot;
  MenuItem* setNotice = menu_item_init(set, "Notice settings");
  menu_item_init(setNotice, "Test button");
  menu_item_init(setNotice, "Update info");
  menu_item_init(set, "Pair button");
  menu_item_init(set, "Disable system");
  menu_item_init(root, "Activity (%dh)");
  menu_item_init(root, "Network (%dh)");
  menu_item_init(root, "Button (%dh)");

  menu->current = menu->root->child;
  return(menu);
}


void* jumpToRoot(Menu* menu) {
  menu->current = menu->root->child;
  return(NULL);
}
