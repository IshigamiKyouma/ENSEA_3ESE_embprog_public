/*
 * app.h
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#ifndef TD3_COMPLETE_APP_APP_H_
#define TD3_COMPLETE_APP_APP_H_

#include "bsp.h"
#include "menu.h"
#include <stdio.h>

typedef struct {
	bsp_t *bsp;
	menu_t menu;
	bool led_on[3];
} app_t;

void app_init(app_t *app, bsp_t *bsp);
void app_process(app_t *app);


#endif /* TD3_COMPLETE_APP_APP_H_ */
