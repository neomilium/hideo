#include <avr/pgmspace.h>

#include "app_keyboard.h"
#include "keyboard.h"

#include "lcd.h"
#include "relay.h"
#include "windowmanager.h"
#include "menus.h"

static uint8  _menu_current_item = 0;
static menu_t  *_menu;

void
_app_hqi_init(void *menu)
{
  _menu = menu;
  menus_display(_menu, _menu_current_item);
}

void
_app_hqi_event_handler(const event_t event)
{
  if (E_KEY_PRESSED == event.code) {
    switch (event.data) {
        case KEYBOARD_UP:
        _menu_current_item = (_menu_current_item - 1 + _menu->item_count) % _menu->item_count;
        menus_display(_menu, _menu_current_item);
        break;
      case KEYBOARD_DOWN:
        _menu_current_item = (_menu_current_item + 1) % _menu->item_count;
        menus_display(_menu, _menu_current_item);
        break;
      case KEYBOARD_MENU_LEFT:
        windowmanager_exit();
        break;
      case KEYBOARD_MENU_RIGHT:
        if (_menu_current_item == 0)
          RELAY0 = 1;
        else
          RELAY0 = 0;
      break;
      default:
      break;
    }
  }
}


