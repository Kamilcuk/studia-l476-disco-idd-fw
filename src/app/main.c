/*
 * main.c
 *
 *  Created on: 23 kwi 2018
 *      Author: kamil
 */
#include <machine/mx.h>
#include <uni/pwrmode.h>
#include <uni/gpio.h>
#include <uni/wdg.h>
#include <unistd_ex.h>
#include <stddef.h>
#include <cdefs.h>
#include <machine/hal_dmafiforx.h>

static uint32_t hadc_dma_buf[2000];
static struct HAL_DMAfiforx_s hadc_dmafiforx	=
		HAL_DMAI_INIT32(hadc_dma_buf, __arraycount(hadc_dma_buf));

typedef unsigned int adc_val_t;
typedef int adc_t;
void adc_init(adc_t t);
uint32_t adc_get(adc_t t);

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

uint32_t adc_get(adc_t t)
{
	adc_init(t);
	uint32_t temp;
	size_t ret;
	PWRMODE_WHILE(PWRMODE_SLEEP,
			(ret = HAL_DMAfiforx_read32(&hadc_dmafiforx, __HAL_DMA_GET_COUNTER(hadc.DMA_Handle), &temp, 1)) == 0);
	printf("%d\n", hadc_dmafiforx.pos/4);
	return HAL_DMAfiforx_getdata32(&hadc_dmafiforx)[0];
}

enum {
	MEAS_MODE_1_COEF,
	MEAS_MODE_24_COEF,
	MEAS_MODE_620_COEF,
	MEAS_MODE_10K_COEF,
};
enum meas_mode_e {
	MEAS_MODE_1 = 1,
	MEAS_MODE_24 = 2,
	MEAS_MODE_620 = 3,
	MEAS_MODE_10K = 4,
};
static const struct {
	enum meas_mode_e mode;
	unsigned long ths_up;
	unsigned long ths_down;
} meas_modes[] = {
	{ MEAS_MODE_1, 100, 200 },
	{ MEAS_MODE_24, 100, 200 },
	{ MEAS_MODE_620, 100, 200 },
	{ MEAS_MODE_10K, 100, 200 },
};

struct meas_s {
	enum meas_mode_e mode;
	unsigned long vals[5];
	unsigned long mean;
};

void meas_init(struct meas_s *t);
void meas_set_mode(struct meas_s *t, enum meas_mode_e mode);

static inline
unsigned long meas_meas_to_ma(struct meas_s *t, uint32_t meas) {
	return meas*t->mode;
}
static inline
uint32_t meas_meas_from_ma(struct meas_s *t, unsigned long ma) {
	return ma/t->mode;
}

void meas_init(struct meas_s *t) {
	memset(t, 0, sizeof(*t));
	meas_set_mode(t, MEAS_MODE_10K);
}

void meas_set_mode(struct meas_s *t, enum meas_mode_e mode)
{
	if (t->mode == mode) return;
	struct bool4_s {
		bool sh0, cal, sh1, sh2;
	} vals;
	switch(mode) {
	case MEAS_MODE_1:   vals = (struct bool4_s){0,1,1,1}; break;
	case MEAS_MODE_24:  vals = (struct bool4_s){1,0,0,1}; break;
	case MEAS_MODE_620: vals = (struct bool4_s){1,0,1,0}; break;
	case MEAS_MODE_10K: vals = (struct bool4_s){1,0,1,1}; break;
	default: assert(0); break;
	}
	gpio_set(gpio_sh0, vals.sh0);
	gpio_set(gpio_cal, vals.cal);
	gpio_set(gpio_sh1, vals.sh1);
	gpio_set(gpio_sh2, vals.sh2);
}

unsigned long meas_calc_mean(struct meas_s *t) {
	assert(t);
	unsigned long sum = 0;
	unsigned long *start = &t->vals[0];
	unsigned long *stop = &t->vals[0];
	for(;start != stop; ++start) {
		sum += *start;
	}
	return sum/__arraycount(t->vals);
}

unsigned long meas_get_mean(struct meas_s *t) {
	assert(t);
	return t->mean;
}

enum meas_mode_e meas_decide_next_mode(struct meas_s *t)
{
	size_t i = 0;
	for(i = 0; i < __arraycount(meas_modes); ++i) {
		if (meas_modes[i].mode == t->mode) {
			break;
		}
	}
	assert(0 <= i && i < __arraycount(meas_modes));
	if (t->mean < meas_modes[i].ths_down) {
		if (i != 0) {
			--i;
		}
	} else if (t->mean > meas_modes[i].ths_up) {
		if (i != __arraycount(meas_modes)-1) {
			++i;
		}
	}
	assert(0 <= i && i < __arraycount(meas_modes));
	return meas_modes[i].mode;
}

void meas_add_meas(struct meas_s *t, uint32_t meas)
{
	assert(t);
	memcpy(t->vals, &t->vals[1], sizeof(t->vals)-sizeof(t->vals[1]));
	t->vals[__arraycount(t->vals)-1] = meas_meas_to_ma(t, meas);
	t->mean = meas_calc_mean(t);
}

int main(void)
{
	struct meas_s meas;
	printf("------------------\n");
	if (wdg_get()) printf("WATCHDOG!\n");
	meas_init(&meas);
	for(;;wdg_refresh()) {
		uint32_t val = adc_get(0);
		meas_add_meas(&meas, val);
		enum meas_mode_e meas_mode = meas_decide_next_mode(&meas);
		meas_set_mode(&meas, meas_mode);
		printf("adc_get=%ld %08lx measurement=%ld\n", val, val, meas_get_mean(&meas));
	}
	return 0;
}
