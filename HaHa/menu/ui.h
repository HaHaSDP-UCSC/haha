/**
 * @file init.h
 * @brief User interface
 * @author August Valera (avalera)
 * @version
 * @date 2017-03-07
 */

#ifndef _HA_INIT_
#define _HA_INIT_

#include "utils/hahalib.h"
#include "menu.h"
// #include "base.h"

// Global variables
Menu *menu;
char *listenPort;
char *destinationPort;
MenuItem *eventButton;

Menu* ui_init(void);
void* jumpToRoot(Menu* menu);

#endif // _HA_INIT_
