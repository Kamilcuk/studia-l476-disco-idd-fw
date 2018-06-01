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

#include <uni/adc.h>
#include <machine/mx.h>
#include <machine/hal_dmafiforx.h>
#include <cdefs.h>
#include <uni/pwrmode.h>
#include <uni/uni.h>

#include "meas.h"

#define PRIF(res)  "%ld.%0" #res "ld"
#define PRIFARGS_IN(val, divisor)  \
	((long)(val)), ((long)((double)(divisor) * (val))%(long)(divisor))
#define PRIFARGS(res, val) \
		PRIFARGS_IN((val), ( \
				(res)==0?1: \
				(res)==1?10: \
				(res)==2?100: \
				(res)==3?1000: \
				(res)==4?10000: \
				(res)==5?100000: \
				(res)==6?1000000: \
				(res)==7?10000000: \
				(res)==8?100000000: \
				(res)==9?1000000000: \
				(res)))

int main(void)
{
	uni_init();
	printf("------------------\n");
	if (wdg_get()) printf("WATCHDOG!\n");
	struct meas_s meas;
	meas_init(&meas);
	adc_on(0);
	for(;;wdg_refresh()) {
		const adc_val_t val = adc_get(0);
		(void)meas_handleNewData(&meas, val.vref, val.val);
		printf("%d " PRIF(9) " " PRIF(9) "\n",
				meas.mode,
				PRIFARGS(9, val.vref),
				PRIFARGS(9, val.val)
		);
		fsync(STDOUT_FILENO);
	}

	return 0;
}
