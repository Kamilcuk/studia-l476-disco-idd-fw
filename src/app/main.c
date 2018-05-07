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

#include "meas.h"

static uint32_t hadc_dma_buf[5];
static struct HAL_DMAfiforx_s hadc_dmafiforx =
		HAL_DMAFIFORX_INIT32(hadc_dma_buf, __arraycount(hadc_dma_buf));
static volatile bool ready = false;

void adc_init(adc_t t)
{
	assert(t == 0);
	if (READ_BIT(hadc.State,HAL_ADC_STATE_READY)) {
		HAL_DMAfiforx_reset(&hadc_dmafiforx);
		HAL_try( HAL_ADC_Start_DMA(&hadc,
				HAL_DMAfiforx_getdata32(&hadc_dmafiforx),
				HAL_DMAfiforx_getsize32(&hadc_dmafiforx)) );
	}
}

adc_val_t adc_get(adc_t t)
{
	//adc_init(t);
	//uint32_t temp;
	//size_t ret;
	//PWRMODE_WHILE(PWRMODE_RUN,
	//while((ret = HAL_DMAfiforx_read32(&hadc_dmafiforx, __HAL_DMA_GET_COUNTER(hadc.DMA_Handle), &temp, 1)) == 0);
#if 1
	//HAL_StatusTypeDef status;
	//while((status = HAL_ADC_PollForConversion(&hadc, UINT32_MAX)) != HAL_OK);
	//assert(status == HAL_OK);
	static bool once = false;
	if (!once) {
		once = true;
		HAL_try( HAL_ADC_Start_IT(&hadc) );
	}
	ready = false;
	while(ready == false);
	const uint32_t temp = HAL_ADC_GetValue(&hadc);
#endif
	return temp;
}
static volatile uint32_t tmp;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ready = true;
}

int main(void)
{
	printf("------------------\n");
	if (wdg_get()) printf("WATCHDOG!\n");
	struct meas_s meas;
	meas_init(&meas);
	for(;;wdg_refresh()) {

		const uint32_t val = adc_get(0);
		meas_add_meas(&meas, val);
		printf("adc_get=%ld %08lx mean=%ld\n", val, val, (unsigned long)(1000000.*meas_get_mean(&meas)));

	}

	return 0;
}
