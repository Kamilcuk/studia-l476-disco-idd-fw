/*
 * meas.h
 *
 *  Created on: 6 maj 2018
 *      Author: kamil
 */

#ifndef SRC_APP_MEAS_H_
#define SRC_APP_MEAS_H_

#include <stdint.h>

#define MEAS_RESOLUTION  4096 // ADC resolution is equal to 2^12
#define MEAS_AMP_BETA    (6040./(6040.+300000.)) // TSZ122IST op-amp beta value, R1=300K R2=6K04 1/beta=~50

enum meas_mode_e {
	MEAS_MODE_NONE = 0,
	MEAS_MODE_1 = 1,
	MEAS_MODE_24 = 24,
	MEAS_MODE_620 = 620,
	MEAS_MODE_10K = 10000,
};

struct meas_s {
	enum meas_mode_e mode;
	double vals[5];
	double mean;
};

void meas_init(struct meas_s *t);
void meas_set_mode(struct meas_s *t, enum meas_mode_e mode);
double meas_calc_mean(struct meas_s *t);
double meas_get_mean(struct meas_s *t);
enum meas_mode_e meas_decide_next_mode(struct meas_s *t);
void meas_add_meas(struct meas_s *t, uint32_t meas);
double meas_meas_to_a(struct meas_s *t, unsigned long meas);

#endif /* SRC_APP_MEAS_H_ */
