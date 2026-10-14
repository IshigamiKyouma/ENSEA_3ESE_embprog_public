/*
 * app.c
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#include "app.h"

static const char *const LED_LABELS[3] = {"LED1", "LED2", "LED3"};

void app_init(app_t *app, bsp_t *bsp) {
	app->bsp = bsp;
	menu_init(&app->menu);
	for (int i = 0; i < 3; i++) {
		app->led_on[i] = false;
		menu_entry_t entry = { LED_LABELS[i], &app->led_on[i], (uint8_t)i };
		menu_add_entry(&app->menu, entry);
	}
}
void app_process(app_t *app) {
	menu_process(&app->menu, app->bsp);
	menu_draw(&app->menu, app->bsp);
}
