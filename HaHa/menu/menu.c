/**
 * @file menu.c
 * @brief Menu and MenuItem library functions
 * @author August Valera (avalera)
 * @version
 * @date 2017-02-28
 */

#include "menu.h"

Menu* menu_init(void) {
  Menu* this = calloc(1, sizeof(Menu));
  this->root = menu_item_init(NULL, "__ROOT__");
  this->root->active = false;
  this->current = NULL;
  return(this);
}

int menu_move(Menu* menu, MenuDirection direct) {
  if(menu && menu->current) {
    switch(direct) {
      case MENU_LEFT:
        // Root node should not be accessible
        if(menu->current->parent && menu->current->parent->parent) {
          menu->current = menu->current->parent;
        } else return(1);
        break;
      case MENU_RIGHT:
        menu->current->onClick(menu);
        break;
      case MENU_UP:
        menu->current = menu_item_get_prev(menu->current);
        break;
      case MENU_DOWN:
        menu->current = menu_item_get_next(menu->current);
        break;
      default:
        return(-1);
        break;
    }
  } else return(-1);
  menu->current->onView(menu);
  return(0);
}

int menu_set_lcd(Menu* menu) {
  int lines = 0;
  if(menu && menu->current) {
    lcd_clear();
    MenuItem* this = menu->current;
    for(lines = 0; lines < LCD_ROWS; lines++) {
      lcd_set_line(lines, this->value);
      if(this->child) {
        if(lines == 0) lcd_set_char(lines, LCD_COLS - 1, MENU_CHAR_SEL_CHLD);
        else lcd_set_char(lines, LCD_COLS - 1, MENU_CHAR_CHLD);
      } else if(this->onClick != menu_item_on_click_default) {
        if(lines == 0) lcd_set_char(lines, LCD_COLS - 1, MENU_CHAR_SEL_FUNC);
        else lcd_set_char(lines, LCD_COLS - 1, MENU_CHAR_FUNC);
      }
      this = menu_item_get_next(this);
      if(this == menu->current) break;
    }
  } else return(-1);
  return(lines);
}

int menu_destroy(Menu* this) {
  if(this) {
    menu_item_destroy(this->root);
    free(this);
  } else return(-1);
  return(0);
}

MenuItem* menu_item_init(MenuItem* parent, char* value) {
  MenuItem* this = calloc(1, sizeof(MenuItem));
  this->onView = menu_item_on_view_default;
  this->onClick = menu_item_on_click_default;
  menu_item_set_value(this, value);
  this->active = true;
  this->parent = parent;
  this->child = NULL;
  this->next = NULL;
  this->prev = NULL;
  if(this->parent) { // Should be true except for root
    if(this->parent->child) { // Not first child
      this->prev = this->parent->child;
      while(this->prev->next) this->prev = this->prev->next;
      this->prev->next = this;
    } else {
      this->parent->child = this;
    }
  }
  return(this);
}

int menu_item_set_value(MenuItem* this, char* value) {
  if(this && value) {
    strcpy(this->value, value);
  } else return(-1);
  return(0);
}

void menu_item_print_tree(MenuItem* this) {
  if(this)
    menu_item_print_tree_helper(this, 0);
}

void menu_item_print_tree_helper(MenuItem* this, int level) {
  if(this && level >= 0) {
    int i;
    for(i = 0; i < level; i++)
      printd("\t");
    if(this->active) {
      printd("* ");
    } else printd("X ");
    printd("%s\n", this->value);
    MenuItem* child;
    for(child = this->child; child; child = child->next)
      menu_item_print_tree_helper(child, level + 1);
  }
}

MenuItem* menu_item_get_next(MenuItem* this) {
  if(this && this->parent) {
    MenuItem* current = this->next;
    while(current != this) {
      if(! current) {
        if(MENU_WRAP) current = this->parent->child;
        else return(this);
      }
      if(current->active) return(current);
      current = current->next;
    }
  }
  return(this);
}

MenuItem* menu_item_get_prev(MenuItem* this) {
  if(this && this->parent) {
    MenuItem* current = this->prev;
    while(current != this) {
      if(! current) {
        if(MENU_WRAP) current = menu_item_get_last(this);
        else return(this);
      }
      if(current->active) return(current);
      current = current->prev;
    }
  }
  return(this);
}

MenuItem* menu_item_get_last(MenuItem* this) {
  MenuItem* last;
  if(this) for(last = this; last->next; last = last->next);
  else return(this);
  return(last);
}

int menu_item_destroy(MenuItem* this) {
  if(this) {
    printd("DELETE %s\n", this->value);
    while(this->child) {
      menu_item_destroy(this->child);
    }
    if(this->prev) { // Not first child
      this->prev->next = this->next;
    } else {
      if(this->next) this->next->prev = NULL;
      if(this->parent) this->parent->child = this->next;
    }
    free(this);
  } else return(-1);
  return(0);
}

void* menu_item_on_view_default(Menu* menu) {
  menu_set_lcd(menu);
  lcd_update();
  return NULL;
}

void* menu_item_on_click_default(Menu* menu) {
  if(menu && menu->current) {
    if(menu->current->child) {
      menu->current = menu->current->child;
    } else return(1);
  } else return(-1);
  return(0);
}
