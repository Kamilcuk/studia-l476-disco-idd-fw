/*
 * main.c
 *
 *  Created on: 23 kwi 2018
 *      Author: kamil
 */
#include <uni/adc.h>
#include <uni/wdg.h>
#include <unistd_ex.h>
#include <stddef.h>
#include <cdefs.h>
#include <uni/gpio.h>
#include <breakpoint.h>
#include <machine/rtc.h>

#include <uni/adc.h>
#include <machine/mx.h>
#include <machine/hal_dmafiforx.h>
#include <cdefs.h>
#include <uni/pwrmode.h>
#include <uni/uni.h>

#include "meas.h"
#include "prif.h"

extern uint32_t htim5_counter_high; // from uni/adc.c

int main(void)
{
	uni_init();
	printf("------------------\n");
	if (wdg_get()) printf("WATCHDOG!\n");
	struct meas_s meas;
	meas_init(&meas);
	adc_on(0);
	for (;;wdg_refresh()) {
		const adc_val_t val = adc_get(0);
		(void)meas_handleNewData(&meas, val.vref, val.val);
		fsync(STDOUT_FILENO);
		printf("%lu %lu %d " PRIF(9) " " PRIF(9) "\n",
				htim5_counter_high,
				__HAL_TIM_GET_COUNTER(&htim5),
				meas.mode,
				PRIFARG(9, val.vref),
				PRIFARG(9, val.val)
		);
	}

	return 0;
}
