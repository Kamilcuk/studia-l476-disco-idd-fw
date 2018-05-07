/*
 * adc.h
 *
 *  Created on: 6 maj 2018
 *      Author: kamil
 */

#ifndef SRC_APP_UNI_ADC_H_
#define SRC_APP_UNI_ADC_H_

#include <uni/types.h>

typedef unsigned long adc_val_t;
typedef int adc_t;

void adc_init(adc_t t);
adc_val_t adc_get(adc_t t);

#endif /* SRC_APP_UNI_ADC_H_ */
