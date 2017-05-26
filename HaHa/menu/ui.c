/**
 * @file init.c
 * @brief User interface
 * @author August Valera (avalera)
 * @version
 * @date 2017-03-07
 */

#include "ui.h"

void ui_init(void) {
  MenuItem* temp;
  menu = menu_init();
  MenuItem* root = menu->root;
  MenuItem* mm = ui_item_init(root, "Main Menu");
  MenuItem* contacts = ui_item_init(mm, "Contact list");
  MenuItem* user = ui_item_init(mm, "User info");
  temp = ui_item_init(user, "__USERPORT__");
  temp->onView = ui_onview_userinfo;
  temp->onClick = ui_onclick_userinfo;
  temp = ui_item_init(user, "__USERNAME__");
  temp->onView = ui_onview_userinfo;
  temp->onClick = ui_onclick_userinfo;
  temp = ui_item_init(user, "__USERADDR__");
  temp->onView = ui_onview_userinfo;
  temp->onClick = ui_onclick_userinfo;
  temp = ui_item_init(user, "__USERCALL__");
  temp->onView = ui_onview_userinfo;
  temp->onClick = ui_onclick_userinfo;
  MenuItem* set = ui_item_init(mm, "Device settings");
  ui_item_init(set, "Office mode");
  MenuItem* setAlert = ui_item_init(set, "Alert settings");
  ui_item_init(setAlert, "Sound");
  ui_item_init(setAlert, "Lights");
  MenuItem* setNotice = ui_item_init(set, "Notice settings");
  ui_item_init(setNotice, "Test button");
  ui_item_init(setNotice, "Update info");
  ui_item_init(set, "Pair button");
  ui_item_init(set, "Disable system");
  ui_item_init(root, "Activity (%dh)");
  ui_item_init(root, "Net (%dh)");
  ui_item_init(root, "Button (%dh)");
  menu->current = menu->root->child;
}

void ui_move(MenuDirection direct) {
	menu_move(menu, direct);
}

int ui_set_lcd(void) {
	menu_set_lcd(menu);
	lcd_update();
}

void ui_update(void) {
	if(! gpio_get_pin_level(BTN_DOWN)) {
		menu_move(menu, MENU_DOWN);
		} else if(! gpio_get_pin_level(BTN_RIGHT)) {
		menu_move(menu, MENU_RIGHT);
		} else if(! gpio_get_pin_level(BTN_UP)) {
		menu_move(menu, MENU_UP);
		} else if(! gpio_get_pin_level(BTN_LEFT)) {
		menu_move(menu, MENU_LEFT);
	}
}

MenuItem* ui_item_init(MenuItem* parent, char* value) {
	MenuItem* this = menu_item_init(parent, value);
	this->onView = ui_item_onview_default;
	this->onClick = ui_item_onclick_default;
	return(this);
}

void* ui_item_onview_default(Menu* menu) {
  ui_set_lcd();
  return;
}

void* ui_item_onclick_default(Menu* menu) {
  if(menu && menu->current) {
    if(menu->current->child) {
      menu->current = menu->current->child;
    } else return(1);
  } else return(-1);
  return(0);
}

void* ui_onview_userinfo(Menu* menu) {
	lcd_clear();
	if(streq(menu->current->value, "__USERPORT__")) {
		lcd_set_line(0, "Your Base ID");
		lcd_set_line(1, "PLACEHOLDER");
	} else if(streq(menu->current->value, "__USERNAME__")) {
		lcd_set_line(0, "Your Name");
		lcd_set_line(1, "PLACEHOLDER");
	} else if(streq(menu->current->value, "__USERADDR__")) {
		lcd_set_line(0, "Your Address");
		lcd_set_line(1, "PLACEHOLDER");
	} else if(streq(menu->current->value, "__USERCALL__")) {
		lcd_set_line(0, "Your Phone #");
		lcd_set_line(1, "PLACEHOLDER");
	}
	lcd_set_line(3, "v MORE    EDIT >");
	lcd_update();
}

void* ui_onclick_userinfo(Menu* menu) {
	if(streq(menu->current->value, "__USERPORT__")) {
		
	} else if(streq(menu->current->value, "__USERNAME__")) {
		char name[1024];
		bzero(&name, sizeof(name));
		ui_input_init(menu, "Edit Name:", "abcdefg", &name);
		lcd_set_line(0, "");
		lcd_set_line_overflow(1, name);
	} else if(streq(menu->current->value, "__USERADDR__")) {
	
	} else if(streq(menu->current->value, "__USERCALL__")) {

	}
}

void ui_input_init(Menu* menu, char* value, char* letters, char** output) {
	MenuItem* inputRoot = ui_item_init(menu->current, value);
	inputRoot->onView = ui_input_onview;
	char letterValue[2];
	bzero(&letterValue, sizeof(letterValue));
	letterValue[0] = UI_INPUT_ENTER;
	MenuItem* inputEnter = ui_item_init(inputRoot, letterValue);
	inputEnter->onView = ui_input_onview;
	inputEnter->onClick = ui_input_onclick;
	for(int i = 0; i < strlen(letters); i++) {
		letterValue[0] = letters[i];
		MenuItem* letter = ui_item_init(inputRoot, letterValue);
		letter->onView = ui_input_onview;
		letter->onClick = ui_input_onclick;
	}
	menu->inputBuffer = malloc(1024 * sizeof(char));
	menu->current = inputEnter;
}

void ui_input_save(Menu* menu) {
	strcpy(menu->outputBuffer, menu->inputBuffer);
	ui_input_destroy(menu);
}

void ui_input_delete(Menu* menu) {
	char* inputBuffer = menu->inputBuffer;
	if(strlen(inputBuffer) > 0) {
		inputBuffer[strlen(inputBuffer)] = NULL;
		menu_move(menu, MENU_RIGHT);
	} else ui_input_destroy(menu);
}

void ui_input_destroy(Menu* menu) {
	 MenuItem* inputRoot = menu->current;
	 free(menu->inputBuffer);
	 menu->inputBuffer = NULL;
	 menu->outputBuffer = NULL;
	 menu_move(menu, MENU_LEFT);
	 menu_item_destroy(inputRoot);
}

void* ui_input_onview(Menu* menu) {
	char* currentValue = menu->current->value;
	if(strlen(currentValue) == 1) {
		char* inputBuffer = menu->inputBuffer;
		int inputLen = strlen(inputBuffer);
		inputBuffer[inputLen] = currentValue[0];
		lcd_set_line(0, menu->current->parent->value);
		lcd_set_line_overflow(1, inputBuffer);
		inputBuffer[inputLen] = NULL;
	} else {
		// Current is input root
		ui_input_delete(menu);
	}
}

void* ui_input_onclick(Menu* menu) {
	if(menu->current->value[0] != UI_INPUT_ENTER) {
		menu->inputBuffer[strlen(menu->inputBuffer)] = menu->current->value[0];
		menu->current = menu->current->parent->child;
	} else {
		// Current is enter key
		menu->current = menu->current->parent;
		ui_input_save(menu);
	}
}

/*
void ui_user_input(MenuItem* item, char* value, char** output) {
	MenuItem* input = ui_item_init(user, value);
	char value[2];
	bzero(&value, sizeof(value));
	char[] letters = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 
		'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'.', '-', ' ', UI_INPUT_ENTER};
	for (char letter : letters) {
		value[0] = letter;
		MenuItem* temp = ui_item_init(input, value);
		temp->onView = ui_user_input_onview;
		temp->onClick = ui_user_input_onclick;
	}
}

void* ui_user_input_onview(Menu menu) {
	lcd_set_line(0, menu->current->parent->value);
	lcd_set_line_overflow(1, menu->inputBuffer);
}

void* ui_user_input_onclick(Menu menu) {
	if(menu->current->value[0] == UI_INPUT_ENTER) {
		
	} else {
		
	}
}
*/