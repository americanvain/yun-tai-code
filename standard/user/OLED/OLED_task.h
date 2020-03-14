#ifndef _OLED_TASK_H_
#define _OLED_TASK_H_

#include <stdint.h>

#define TITLE_LEN 12
typedef struct _Menu_option
{
	char title[TITLE_LEN];
	struct _Menu_option *nextoption;
	struct _Menu_option *submenu;
	struct _Menu_option *parentmenu;
	void (*hook)(void* param);
}Menu_option;

void OLED_Prc(void);
uint8_t OLED_Button(void);

void menu_init(void);
Menu_option *menu_option_creat(char title[TITLE_LEN],Menu_option *previous_option,void (*hook)(void* param));
Menu_option *submenu_creat(char title[TITLE_LEN],Menu_option *parentmenu,void (*hook)(void* param));
void menu_switch(void);
void menu_display(void);
void cursor_display(void);
void show_logo_hook(void*);
void show_time_hook(void*);
void show_time(void);
#endif
