/*
 * app.c
 *
 *  Created on: Sep 30, 2026
 *      Author: johann
 */
#include "app.h"

void app_process(app_t *app) {
	uint32_t tick = bsp_get_tick_ms();
	if (tick - app->last_tick >= 250) {
		app->last_tick = tick;
		app->led1_on = !app->led1_on;
		bsp_led_set(0, app->led1_on);
	}
	if (bsp_sw1_pressed()) {
			app->led2_on = !app->led2_on;
			bsp_led_set(0, app->led2_on);
	}
	if (bsp_sw2_pressed()) {
			app->led3_on = !app->led3_on;
			bsp_led_set(0, app->led3_on);
	}
	/* led2_on/led3_on on bsp_sw1_pressed()/bsp_sw2_pressed(): same shape */
}
