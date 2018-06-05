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
#include <machine/rtc.h>
#include <cdefs.h>
#include <breakpoint.h>
#include <stdbool.h>
#include <uni/rtc.h>
#include <assert.h>

static volatile enum {
	S_RESET, // reset state
	S_DATA_READY, // HAL_ADC_ConvCpltCallback called, waiting for data to be read from ADC registers
	S_DATA_READED, // data read from registers, ADC maybe rearmed in HAL_TIM_PeriodElapsedCallback
	S_ARMED, // HAL_ADC is armed, HAL_ADC_ConvCpltCallback should be called
} state = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
}

uint32_t htim5_counter_high = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim == &htim2) {
		HAL_try(HAL_ADC_Start(&hadc));
		assert(state == S_DATA_READED);
		state = S_ARMED;
	} else if (htim == &htim5) {
		++htim5_counter_high;
	}
}

rtc_clock_t rtc_clock(rtc_t t)
{
	return (rtc_clock_t)htim5_counter_high<<32 | (rtc_clock_t)__HAL_TIM_GET_COUNTER(&htim5);
}

void adc_on(adc_t t)
{
	state = S_DATA_READED;
	HAL_try(HAL_TIM_Base_Start_IT(&htim2));
}

static inline adc_val_t adc_get_conv(const double vvolt, const double vrefdata)
{
	const double digital_scale = __LL_ADC_DIGITAL_SCALE(hadc.Init.Resolution);
	const double vrefint_cal = *VREFINT_CAL_ADDR;
	const double vrefint_cal_vref = VREFINT_CAL_VREF;
	const double vref = vrefint_cal * vrefint_cal_vref / 1000. / vrefdata;
	const double val = vvolt * vref / digital_scale;
	const adc_val_t ret = {
			.vref = vref,
			.val = val,
	};
	return ret;
}

adc_val_t adc_get(adc_t t)
{
	assert(state == S_ARMED || state == S_DATA_READED);
	while (state != S_ARMED);
	while (HAL_ADCEx_InjectedPollForConversion(&hadc, HAL_MAX_DELAY));
	state = S_DATA_READY;

	HAL_try(HAL_ADC_Stop(&hadc));
	const uint32_t vvolt32 = HAL_ADC_GetValue(&hadc);
	const uint32_t vref32 = HAL_ADCEx_InjectedGetValue(&hadc, ADC_INJECTED_RANK_1);
	assert(state == S_DATA_READY);
	state = S_DATA_READED;

	return adc_get_conv(vvolt32, vref32);
}


