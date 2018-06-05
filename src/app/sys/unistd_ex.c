/*
 * unistd_ex.c
 *
 *  Created on: 3 cze 2018
 *      Author: kamil
 */

#include <machine/hal.h>
#include <unistd_ex.h>

void mdelay(time_ms_t ms) {
	HAL_Delay(ms);
}
