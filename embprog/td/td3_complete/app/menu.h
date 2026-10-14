/*
 * menu.h
 *
 *  Created on: Oct 1, 2026
 *      Author: johann
 */

#ifndef STM32CUBEMX_APP_MENU_H_
#define STM32CUBEMX_APP_MENU_H_

#include <stdio.h>
#include <stdbool.h>
#include "bsp.h"

typedef struct {
	const char *label;
	bool *led_on;
	uint8_t led_index;
} menu_entry_t;

void menu_entry_toggle(menu_entry_t *e, bsp_t *bsp);
typedef struct {
	menu_entry_t entries[4];
	int count;
	int cursor;
} menu_t;
void menu_init(menu_t *m);
void menu_add_entry(menu_t *m, menu_entry_t entry);
void menu_process(menu_t *m, bsp_t *bsp);
void menu_draw(menu_t *m, bsp_t *bsp);
#endif /* STM32CUBEMX_APP_MENU_H_ */
