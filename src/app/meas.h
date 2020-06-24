
/*
 * meas.h
 *
 *  Created on: 6 maj 2018
 *      Author: kamil
 */

#ifndef SRC_APP_MEAS_H_
#define SRC_APP_MEAS_H_

#include <stdint.h>

enum meas_mode_e {
	MEAS_MODE_1 = 0,
	MEAS_MODE_24 = 1,
	MEAS_MODE_620 = 2,
	MEAS_MODE_10K = 3,
};
#define MEAS_MODE_CNT  4

struct meas_s {
	enum meas_mode_e mode;
	double volt[5];
};

static const double meas_amp_beta = ( 6040. / ( 6040. + 300000. ) );
static const double meas_Uref = ( 15000. / ( 300000. + 15000. ) ) * 3.3;
static const double meas_Rm[MEAS_MODE_CNT] = {
		1. / ( ( 1. / 1. ) + ( 1. / 10000. ) ) ,
		1. / ( ( 1. / 24. ) + ( 1. / 10000. ) ) ,
		1. / ( ( 1. / 620. ) + ( 1. / 10000. ) ) ,
		10000. ,
};

void meas_init(struct meas_s *t);
void meas_setMode(struct meas_s *t, enum meas_mode_e mode);
double meas_handleNewData(struct meas_s *t, double vref, double val);

#endif /* SRC_APP_MEAS_H_ */
