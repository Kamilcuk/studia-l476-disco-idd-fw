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

void meas_init(struct meas_s *t) {
	memset(t, 0, sizeof(*t));
	meas_set_mode(t, MEAS_MODE_10K);
}

void meas_set_mode(struct meas_s *t, enum meas_mode_e mode)
{
	assert(t);
	if (t->mode == mode) return;
	struct bool4_s {
		bool cal, sh0, sh1, sh2;
	} vals = {0};
	switch(mode) {
	case MEAS_MODE_1:   vals = (struct bool4_s){0,0,1,1}; break;
	case MEAS_MODE_24:  vals = (struct bool4_s){0,1,0,1}; break;
	case MEAS_MODE_620: vals = (struct bool4_s){0,1,1,0}; break;
	case MEAS_MODE_10K: vals = (struct bool4_s){0,1,1,1}; break;
	default: assert(0); break;
	}
	gpio_set(gpio_cal, vals.cal);
	gpio_set(gpio_sh0, vals.sh0);
	gpio_set(gpio_sh1, vals.sh1);
	gpio_set(gpio_sh2, vals.sh2);
	t->mode = mode;
}

double meas_calc_mean(struct meas_s *t)
{
	assert(t);
	double sum = 0;
	for(double *i = t->vals[0]; i < &t->vals[__arraycount(t->vals)]; ++i) {
		sum += *i;
	}
	return sum / __arraycount(t->vals);
}

double meas_get_mean(struct meas_s *t)
{
	assert(t);
	return t->mean;
}

enum meas_mode_e meas_decide_next_mode(struct meas_s *t)
{
	assert(0);
	return 0;
}

void meas_add_meas(struct meas_s *t, uint32_t meas)
{
	assert(t);
	memcpy(t->vals, &t->vals[1], sizeof(t->vals)-sizeof(t->vals[1]));
	static_assert(__is_array_of_constant_known_size(t->vals),"");
	t->vals[__arraycount(t->vals)-1] = meas_meas_to_a(t, meas);
	t->mean = meas_calc_mean(t);


}

unsigned long meas_get_Rmeas(struct meas_s *t)
{
	return t->mode;
}

double meas_meas_to_a(struct meas_s *t, unsigned long meas)
{
	const double resolution = MEAS_RESOLUTION;
	const double vdd_ma = 3000.;
	const double umeasraw = meas * vdd_ma / 1000. / resolution;
	const double beta = MEAS_AMP_BETA;
	const double umeas = umeasraw * beta;
	const double imeas = umeas / meas_get_Rmeas(t);
	return imeas;
}
