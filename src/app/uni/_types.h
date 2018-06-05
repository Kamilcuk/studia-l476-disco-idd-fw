/*
 * _types.h
 *
 *  Created on: 3 cze 2018
 *      Author: kamil
 */

#ifndef SRC_APP_UNI__TYPES_H_
#define SRC_APP_UNI__TYPES_H_

typedef struct {
	double vref;
	double val;
} adc_val_t;
#define __adc_val_t_defined 1

typedef unsigned long long rtc_clock_t;
#define __rtc_clock_t_defined 1

#endif /* SRC_APP_UNI__TYPES_H_ */
