/*
 * app.h
 *
 *  Created on: Sep 30, 2026
 *      Author: johann
 */

#ifndef STM32CUBEMX_APP_APP_H_
#define STM32CUBEMX_APP_APP_H_

#include "bsp.h"
/* direct coupling, for now */
typedef struct {
	uint32_t last_tick;
	bool led1_on, led2_on, led3_on;
} app_t;
void app_init(app_t *app);
void app_process(app_t *app);


#endif /* STM32CUBEMX_APP_APP_H_ */
