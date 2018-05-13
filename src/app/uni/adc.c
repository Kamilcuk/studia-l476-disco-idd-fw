/*
 * adc.c
 *
 *  Created on: 6 maj 2018
 *      Author: kamil
 */
#include <uni/adc.h>
#include <machine/mx.h>
#include <machine/stm32xxxx_ll.h>
#include <machine/hal_dmafiforx.h>
#include <uni/pwrmode.h>
#include <cdefs.h>
#include <stdbool.h>
#include <assert.h>

static uint32_t adc_dmabuf[2];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	HAL_try(HAL_ADC_Stop_DMA(hadc));
}

void adc_on(adc_t t)
{
	HAL_try(HAL_ADC_Start_DMA(&hadc, adc_dmabuf, __arraycount(adc_dmabuf)));
}

adc_val_t adc_get(adc_t t)
{
	PWRMODE_WHILE(PWRMODE_SLEEP, ADC_IS_ENABLE(&hadc));
	const double vrefdata = *(volatile uint32_t*)&adc_dmabuf[0];
	const double vvolt = *(volatile uint32_t*)&adc_dmabuf[1];
	adc_on(t);

	assert(hadc.Init.Resolution == ADC_RESOLUTION_12B);
	const double resolution = 4096. - 1.;
	const double vref =
			(double)(*VREFINT_CAL_ADDR) * (double)VREFINT_CAL_VREF / 1000. / vrefdata;
	const double val = vvolt * vref / resolution;
	const adc_val_t ret = {
			.vref = vref,
			.val = val,
	};
	return ret;
}


