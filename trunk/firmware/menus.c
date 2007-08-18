#include "menus.h"

#include "lcd.h"
#include "keyboard.h"
#include "rtc_ds1302.h"

#define MENUS_SIZE    2

t_menu menus[MENUS_SIZE];

/*************************** ROOT MENU ****************************/
const char m_root_menu_date_text[] PROGMEM = "Date";
const char m_root_menu_lcd_text[] PROGMEM = "LCD";
const char m_root_menu_exit_text[] PROGMEM = "Quitter";

#define MENUS_MAX_ENTRIES 2
PGM_P m_root_menu[MENUS_MAX_ENTRIES] PROGMEM = {
	m_root_menu_date_text,
//	m_root_menu_lcd_text,
	m_root_menu_exit_text
};
void (*m_root_functions[MENUS_MAX_ENTRIES]) (void);
uint8 m_root_submenus[MENUS_MAX_ENTRIES];

uint8 menus_current_line;
uint8 menus_current_menu;
uint8 menus_count;
uint8 root_menu_entries_count;

#define MENUS_DISPLAY_MODE_FULL         0
#define MENUS_DISPLAY_MODE_CURSOR_LINE  1

void menus_display(byte mode)
{
	uint8 i;
	uint8 line_count = menus[menus_current_menu].line_count;

	switch(mode) {
		case MENUS_DISPLAY_MODE_FULL: {
				PGM_P line;

				lcd_clear();
				for(i=0; i<line_count; i++) {
					lcd_gotoxy(0,i);
					memcpy_P(&line, &(menus[menus_current_menu].lines)[i], sizeof(PGM_P));
					if(i==menus_current_line)
						lcd_display_char( '>' );
					else
						lcd_display_char( ' ' );
					lcd_display_string( line );
				}
				break;
			case MENUS_DISPLAY_MODE_CURSOR_LINE:
				for(i=0; i<line_count; i++) {
					lcd_gotoxy(0,i);
					if(i==menus_current_line)
						lcd_display_char( '>' );
					else
						lcd_display_char( ' ' );
				}
			}
			break;
	}
	lcd_gotoxy(0,5);
	lcd_display_string( PSTR("m") );
	lcd_display_number( menus_current_menu );
	lcd_display_string( PSTR(",M") );
	lcd_display_number( menus_count );
	lcd_display_string( PSTR(",l") );
	lcd_display_number( menus_current_line );
	lcd_display_string( PSTR(",L") );
	lcd_display_number( line_count );
}

void menus_init(void)
{
	/*** ROOT MENU ***/
	menus[MENU_ROOT].level = 0;
	menus[MENU_ROOT].lines = m_root_menu;
	menus[MENU_ROOT].line_count = 1;
	menus[MENU_ROOT].functions = m_root_functions;
	menus[MENU_ROOT].submenus = m_root_submenus;

	root_menu_entries_count = 1; // "Quitter"
	menus_count = 1; // root_menu
	menus_current_line = 0;
	menus_current_menu = MENU_ROOT;
}

void menus_shutdown_lcd(void)
{
	lcd_clear();
}

void menus_add_submenu(const t_menu submenu)
{
	const uint8 free_entry = root_menu_entries_count;
	root_menu_entries_count++;

//	m_root_menu[free_entry] = text;
	m_root_functions[free_entry] = NULL;
	m_root_submenus[free_entry] = menus_count;
	menus[menus_count] = submenu;
	menus_count++;

	menus[MENU_ROOT].line_count = root_menu_entries_count;
//	m_root_menu[menus_count] = m_root_menu_exit_text;
	m_root_functions[root_menu_entries_count - 1] = menus_shutdown_lcd;
	m_root_submenus[root_menu_entries_count - 1] = MENU_ROOT;

	menus_display(MENUS_DISPLAY_MODE_FULL);
}

void menus_process(void)
{
	byte key = keyboard_key();

	uint8 line_count = menus[menus_current_menu].line_count;
	switch(key) {
		case KEYBOARD_NONE:
			break;
		case KEYBOARD_UP:
				menus_current_line = ( menus_current_line - 1 + line_count ) % line_count;
				menus_display(MENUS_DISPLAY_MODE_CURSOR_LINE);
			break;
		case KEYBOARD_DOWN:
				menus_current_line = ( menus_current_line + 1 ) % line_count;
				menus_display(MENUS_DISPLAY_MODE_CURSOR_LINE);
			break;
		case KEYBOARD_OK:
				if(menus[menus_current_menu].functions[menus_current_line] != NULL)  {
					/* There is a function to execute */
					menus[menus_current_menu].functions[menus_current_line]();
				} else {
					menus_current_menu = menus[menus_current_menu].submenus[menus_current_line];
					menus_current_line = 0;
					menus_display(MENUS_DISPLAY_MODE_FULL);
				}
			break;
/*		case KEYBOARD_CANCEL:
			break;*/
		default:
			menus_display(MENUS_DISPLAY_MODE_FULL);
	}
}
