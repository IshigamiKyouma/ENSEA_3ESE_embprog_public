/*
 * bsp.h
 *
 *  Created on: Sep 30, 2026
 *      Author: johann
 */

#ifndef STM32CUBEMX_BSP_BSP_H_
#define STM32CUBEMX_BSP_BSP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void bsp_init(void);
void bsp_process(void);

void bsp_led_set(uint8_t led, bool on);
bool bsp_sw1_pressed(void);
bool bsp_sw2_pressed(void);
uint32_t bsp_get_tick_ms(void);

#endif /* STM32CUBEMX_BSP_BSP_H_ */
