/**
 * @file init.c
 * @brief User interface
 * @author August Valera (avalera)
 * @version
 * @date 2017-03-07
 */

//#define BRIAN 1

#include "ui.h"
#include "messagequeue/messagequeue.h"
#include "neighbor/friendlist.h"
#include "network/packet.h"

// Character sets
char* ui_charset_alpha = "_abcdefghijklmnopqrstuvwxyz";
char* ui_charset_alphacase = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Accept/Deny screen
void* ui_helpdeny_onview(Menu* menu);
void* ui_helpresp_onview(Menu* menu);
void* ui_helpresp_onclick(Menu* menu);


void ui_helpreq_onclick(Menu *menu){
	//addTestFriend("Brian", "Nichols","0013A200414F50EA");
	//addTestLocalUser("Kevin", "Lee", 0x1);
	//send_ping_request(&friendList[0]);
	
	/* Testing Application code */
	LocalUser self;
	strcpy(self.friend.firstname, "Kevin");
	strcpy(self.friend.lastname, "Lee");
	
	//uint8_t* t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50E9");
	//uint8_t* t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50EA");
#ifndef BRIAN
	uint8_t* t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50E9");
#else
	uint8_t* t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50EA");
#endif
	memcpy(self.friend.networkaddr,t, 8);
	self.friend.port = 0x0001;
	
	Friend f;
	strcpy(f.firstname, "Brian");
	strcpy(f.lastname, "Nichols");
	
	//t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50EA");
	//t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50E9");
	
#ifndef BRIAN
	t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50EA");
#else	
	t = (uint8_t *) convert_asciihex_to_byte("0013A200414F50E9");
#endif	

	memcpy(f.networkaddr, t, 8);
	f.port = 0x0002;
	
	addFriend(&f);
	addLocalUser(&self);
	//send_help_request(&f, &self);
	send_help_request(&friendList[0], &localUsers[0]);
	//send_help_request_ack(Friend *f, LocalUser *self);
	
	bool accept = true;
	//menu->current = menu->current->parent;
}


char ui_global_name[256];

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
  ui_item_helpreq = ui_item_init(root, "SEND HELP");
  ui_item_helpreq->onClick = ui_helpreq_onclick;
  ui_item_helpdeny = ui_item_init(ui_item_helpreq, "_HELP_DENY_");
  ui_item_helpdeny->onView = ui_helpdeny_onview;
  ui_item_helpresp = ui_item_init(ui_item_helpdeny, "_HELP_RESP_");
  ui_item_helpresp->onView = ui_helpresp_onview;
  ui_item_helpresp->onClick = ui_helpresp_onclick;
  menu->current = menu->root->child;
  bzero(&ui_global_name, sizeof(ui_global_name));
  strcpy(ui_global_name, "HOLDPLACER");
}

void* ui_helpdeny_onview(Menu* menu) {
	printd("IN HELPRESP DENY ON VIEW!\n");
	send_help_response(&friendList[0], &localUsers[0], true); //TODO not zero
	menu->current = menu->current->parent; 
	menu->current->onView();
	// Custom code on help request deny

}

void* ui_helpresp_onview(Menu* menu) {
	static uint8_t count = 0;
	printd("In helpresp onview\n");
	lcd_clear();
	lcd_set_line(0, "! HELP REQUEST !");
	printf("before");
	printf("'%s'\n", menu->txtSrc1);
	printBuff(menu->txtSrc1, 3, "%c");
	lcd_set_line(1, menu->txtSrc1);
	lcd_set_line(LCD_ROWS - 1, "<BACK   RESPOND>");
	lcd_update();
	if(count++ > 0)
		menu->current->onView();
	else
		count = 0;
}

void* ui_helpresp_onclick(Menu* menu) {
	printd("IN HELPRESP ON CLICK!\n");
	menu->current = menu->current->parent->parent;
	menu->current->onView();
	send_help_response(&friendList[0], &localUsers[0], true); //TODO not zero
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
		lcd_set_line_overflow(1, ui_global_name);
	} else if(streq(menu->current->value, "__USERADDR__")) {
		lcd_set_line(0, "Your Address");
		lcd_set_line(1, "PLACEHOLDER");
	} else if(streq(menu->current->value, "__USERCALL__")) {
		lcd_set_line(0, "Your Phone #");
		lcd_set_line(1, "PLACEHOLDER");
	}
	lcd_set_line(3, "vMORE      EDIT>");
	lcd_update();
}

void* ui_onclick_userinfo(Menu* menu) {
	printd("ui_onclick_userinfo\n");
	if(streq(menu->current->value, "__USERPORT__")) {
		
	} else if(streq(menu->current->value, "__USERNAME__")) {
		ui_input_init(menu, "Edit Name:", ui_charset_alphacase, &ui_global_name);
	} else if(streq(menu->current->value, "__USERADDR__")) {
	
	} else if(streq(menu->current->value, "__USERCALL__")) {

	}
}

void ui_input_init(Menu* menu, char* value, char* letters, char** output) {
	printd("ui_input_init\n");
	MenuItem* inputRoot = ui_item_init(menu->current, value);
	inputRoot->onView = ui_input_onview;
	char letterValue[2];
	bzero(&letterValue, sizeof(letterValue));
	for(int i = 0; i < strlen(letters); i++) {
		letterValue[0] = letters[i];
		MenuItem* letter = ui_item_init(inputRoot, letterValue);
		letter->onView = ui_input_onview;
		letter->onClick = ui_input_onclick;
	}
	letterValue[0] = UI_INPUT_ENTER;
	MenuItem* inputEnter = ui_item_init(inputRoot, letterValue);
	inputEnter->onView = ui_input_onview;
	inputEnter->onClick = ui_input_onclick;
	menu->inputBuffer = malloc(256 * sizeof(char));
	bzero(menu->inputBuffer, 256);
	menu->outputBuffer = output;
	menu->current = inputRoot->child;
	menu->current->onView();
	lcd_set_line(LCD_ROWS - 1, "Edit with v/^");
}

void ui_input_save(Menu* menu) {
	printd("ui_input_save\n");
	printd("src: '%s', dest: '%s'\n", menu->inputBuffer, menu->outputBuffer);
	strcpy(menu->outputBuffer, menu->inputBuffer);
	ui_input_destroy(menu);
}

void ui_input_delete(Menu* menu) {
	printd("ui_input_delete\n");
	char* inputBuffer = menu->inputBuffer;
	int inputLen = strlen(inputBuffer);
	if(strlen(inputBuffer) > 0) {
		inputBuffer[inputLen - 1] = NULL;
		menu_move(menu, MENU_RIGHT);
	} else ui_input_destroy(menu);
}

void ui_input_destroy(Menu* menu) {
	 printd("ui_input_destroy\n");
	 MenuItem* inputRoot = menu->current;
	 free(menu->inputBuffer);
	 menu->inputBuffer = NULL;
	 menu->outputBuffer = NULL;
	 menu_move(menu, MENU_LEFT);
	 menu_item_destroy(inputRoot);
}

void* ui_input_onview(Menu* menu) {
	printd("ui_input_onview\n");
	char* currentValue = menu->current->value;
	if(strlen(currentValue) == 1) {
		char* inputBuffer = menu->inputBuffer;
		int inputLen = strlen(inputBuffer);
		printd("buf(%d): '%s'\n", inputLen, inputBuffer);
		inputBuffer[inputLen] = currentValue[0];
		lcd_clear();
		lcd_set_line(0, menu->current->parent->value);
		lcd_set_line_overflow(1, inputBuffer);
		lcd_update();
		inputBuffer[inputLen] = NULL;
	} else {
		// Current is input root
		ui_input_delete(menu);
	}
}

void* ui_input_onclick(Menu* menu) {
	printd("ui_input_onclick\n");
	char currentLetter = menu->current->value[0];
	char* inputBuffer = menu->inputBuffer;
	int inputLen = strlen(inputBuffer);
	if(currentLetter == '_') currentLetter = ' ';
	if(currentLetter == UI_INPUT_ENTER) {
		// Current is enter key
		menu->current = menu->current->parent;
		ui_input_save(menu);
	} else {
		inputBuffer[inputLen] = currentLetter;
		menu->current = menu->current->parent->child;
	}
}
