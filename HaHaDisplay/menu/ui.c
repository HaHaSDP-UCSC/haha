/**
 * @file init.c
 * @brief Initialization file
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
/*

// User date code

void* listFriends(Menu* menu) {
    char title[16];
    MenuItem* this = menu->current;
    while(this->child)
        menuItemDestroy(this->child);
    int i, j = 0;
    for(i = 0; i < 128; i++)
        if(self.friends[i] != 0) {
            sprintf(title, "%d: Port %d", j++, self.friends[i]);
            MenuItem* friend = menuItemInit(this, title);
            MenuItem* friendDelete = menuItemInit(friend, "Delete");
            friendDelete->onClick = deleteFriend;
            MenuItem* friendEdit = menuItemInit(friend, "Edit");
            friendEdit->onClick = editFriend;
        }
    MenuItem* addFriend = menuItemInit(this, "Add Friend");
    addFriend->onClick = addFriendInput;
    menuItemOnClickDefault(menu);
    return(NULL);
}

void* editFriend(Menu* menu) {
    int idx = -1, friend = -1;
    if(sscanf(menu->current->parent->value, "%d: ", &idx) != 1) {
        printe("editFriend could not parse index");
        return(NULL);
    }
    friend = self.friends[idx];
    printf("Edit friend %d (currently %d): ", idx, friend);
    if(scanf("%d", &friend) == 1)
        self.friends[idx] = friend;
    writeBase(&self);
    jumpToRoot(menu);
}

void* deleteFriend(Menu* menu) {
    int idx = -1;
    if(sscanf(menu->current->parent->value, "%d: ", &idx) != 1) {
        printe("editFriend could not parse index");
        return(NULL);
    }
    for(;idx < 127; idx++)
        self.friends[idx] = self.friends[idx + 1];
    writeBase(&self);
    jumpToRoot(menu);
}

void* viewUserInfo(Menu* menu) {
    char buffer[LCD_COLS];
    bzero(&buffer, LCD_COLS);
    lcdClear();
    lcdSetLine(3, "v More    Edit >");
    if(streq(menu->current->value, "__USERPORT__")) {
        lcdSetLine(0, "User Port");
        lcdSetLine(1, listenPort);
        lcdSetLine(3, "v More");
    } else if(streq(menu->current->value, "__USERNAME__")) {
        lcdSetLine(0, "User Name");
        lcdSetLine(1, self.firstName);
        lcdSetLine(2, self.lastName);
    } else if(streq(menu->current->value, "__USERADDR__")) {
        lcdSetLine(0, "User Address");
        int len = strlen(self.homeAddr);
        if(len > LCD_COLS) len = LCD_COLS;
        strncpy(buffer, self.homeAddr, len);
        lcdSetLine(1, buffer);
        bzero(&buffer, LCD_COLS);
        if((len = strlen(self.homeAddr)) > LCD_COLS) {
            len -= LCD_COLS;
            if(len > LCD_COLS) len = LCD_COLS;
            strncpy(buffer, &self.homeAddr[LCD_COLS], len);
            lcdSetLine(2, buffer);
        }
    } else if(streq(menu->current->value, "__USERCALL__")) {
        lcdSetLine(0, "User Phone");
        lcdSetLine(1, self.phone);
    } else {
        printe("viewUserInfo called from invalid menu item");
    }
    lcdUpdate();
}

void* editUserInfo(Menu* menu) {
    char buffer[1024];
    bzero(&buffer, 1024);
    if(streq(menu->current->value, "__USERNAME__")) {
        printf("Enter in your first name (currently %s): ", self.firstName);
        fgets(buffer, 17, stdin);
        buffer[strlen(buffer) - 1] = '\0';
        //scanf("%s", buffer);
        if(strlen(buffer) != 0) strncpy(self.firstName, buffer, 17);
        printf("Enter in your last name (currently %s): ", self.lastName);
        fgets(buffer, 17, stdin);
        buffer[strlen(buffer) - 1] = '\0';
        //scanf("%s", buffer);
        if(strlen(buffer) != 0) strncpy(self.lastName, buffer, 17);
    } else if(streq(menu->current->value, "__USERADDR__")) {
        printf("Enter in your address (currently %s): ", self.homeAddr);
        fgets(buffer, 75, stdin);
        buffer[strlen(buffer) - 1] = '\0';
        //scanf("%s", buffer);
        if(strlen(buffer) != 0) strncpy(self.homeAddr, buffer, 75);
    } else if(streq(menu->current->value, "__USERCALL__")) {
        printf("Enter in your address (currently %s): ", self.phone);
        fgets(buffer, 16, stdin);
        buffer[strlen(buffer) - 1] = '\0';
        //scanf("%s", buffer);
        if(strlen(buffer) != 0) strncpy(self.phone, buffer, 16);
    } else {
        printe("viewUserInfo called from invalid menu item");
    }
}

// Bluetooth button code

// @TODO Jamie Call this function (or copy the body of it)
// to trigger the alert screen
void* jumpToEvent(Menu* menu) {
    menu->current = eventButton->child;
    menu->current->onView(menu);
    return(NULL);
}

void* eventButtonView(Menu* menu) {
    char name[LCD_COLS];
    bzero(&name, LCD_COLS);
    sprintf(name, "Port %d", self.dying);
    // Recommendation: put a char[] in the Menu
    // struct to hold the name. Before running the
    // code in jumpToEvent() to display the alert,
    // strcpy() the name of the requester into that
    // char[]. Then you can set it here.
    lcdClear();
    lcdSetLine(0, "HELP REQUEST");
    lcdSetLine(1, name);
    lcdSetLine(2, "Help Them");
    lcdSetLine(3, "Dismiss");
    int this = 3, other = 2;
    if(menu->current->next)
        this = 2, other = 3;
    lcdSetChar(this, LCD_COLS - 1, '>');
    lcdSetChar(other, LCD_COLS - 1, '-');
    lcdUpdate();
    return(NULL);
}

void* eventButtonAnswer(Menu* menu) {
    if(menu->current->next) {
        // Accept
        printd("Help request accepted\n");
        acceptReq = TRUE;
    } else {
        // Deny
        printd("Help request denied :(\n");
        acceptReq = FALSE;
    }
    jumpToRoot(menu);
    return(NULL);
}

void* addFriendInput(Menu* menu) {
    printf("Enter in your friend's port number: ");
    int friend;
    if(scanf("%d", &friend) == 1) {
        if(friend > 0) {
            int i;
            for(i = 0; i < 128; i++)
                if(self.friends[i] == 0)
                    break;
            self.friends[i] = friend;
        } else printe("Invalid port number provided");
    } else printe("Invalid input provided");
    jumpToRoot(menu);
    return(NULL);
}
*/