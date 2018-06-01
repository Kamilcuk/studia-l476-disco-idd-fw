/*
 * meas.c
 *
 *  Created on: 6 maj 2018
 *      Author: kamil
 */
#include <meas.h>
#include <uni/gpio.h>
#include <stdbool.h>
#include <string.h>
#include <cdefs.h>
#include <assert.h>

static inline double accumulate_d(double first[], double *last)
{
	double ret = 0;
	while(first != last) {
		ret += *first++;
	}
	return ret;
}
static inline double mean_d(double arr[], size_t cnt)
{
	return accumulate_d(arr, &arr[cnt]) / (double)cnt;
}

static inline void meas_setMode_callback(enum meas_mode_e mode)
{
	assert(0 <= mode && mode < MEAS_MODE_CNT);
	struct {
		bool cal, sh0, sh1, sh2;
	} vals[MEAS_MODE_CNT] = {
			{0,0,1,1},
			{0,1,0,1},
			{0,1,1,0},
			{0,1,1,1},
	};
	gpio_set(gpio_cal, vals[mode].cal);
	gpio_set(gpio_sh0, vals[mode].sh0);
	gpio_set(gpio_sh1, vals[mode].sh1);
	gpio_set(gpio_sh2, vals[mode].sh2);
}

/* --------------------------------------------------------------- */

void meas_init(struct meas_s *t) {
	memset(t, 0, sizeof(*t));
	meas_setMode(t, MEAS_MODE_10K);
}

void meas_setMode(struct meas_s *t, enum meas_mode_e mode)
{
	assert(t);
	for(size_t i = 0; i < __arraycount(t->volt); ++i) {
		t->volt[i] = 1.5;
	}
	if (t->mode == mode) return;
	meas_setMode_callback(mode);
	t->mode = mode;
}

/**
 ^ VDD/VREF
 |
+++      ^     +--+
| |      |        +--+OPAMP+-^
| | Rm   |Um      |          | Uw
+++      +     +--+          +
 |
+++
| |
| | Rl
+++
 |
 v GND
 */

static inline double meas_Uw_to_ampere(const struct meas_s *t, double val)
{
	return val * meas_amp_beta / meas_Rm[t->mode] + meas_Uref;
}

static inline void meas_chooseNewMode(struct meas_s *t, double vref, double val)
{
	static_assert(__is_array_of_constant_known_size(t->volt),"");
	memmove(t->volt, &t->volt[1], sizeof(t->volt)-sizeof(t->volt[1]));
	t->volt[__arraycount(t->volt)-1] = val;
	const double voltmean = mean_d(t->volt, __arraycount(t->volt));

	const double max = vref * 95. / 100.;
	const double min = vref * 5. / 100.;
	if (voltmean > max) {
		if (t->mode > 0) {
			meas_setMode(t, t->mode-1);
		}
	} else if (voltmean < min) {
		if (t->mode < MEAS_MODE_CNT-1) {
			meas_setMode(t, t->mode+1);
		}
	}
}


double meas_handleNewData(struct meas_s *t, double vref, double val)
{
	assert(t);

	meas_chooseNewMode(t, vref, val);

	return meas_Uw_to_ampere(t, val);
}
