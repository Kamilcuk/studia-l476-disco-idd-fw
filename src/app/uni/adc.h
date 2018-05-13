/*
 * adc.h
 *
 *  Created on: 6 maj 2018
 *      Author: kamil
 */

#ifndef SRC_APP_UNI_ADC_H_
#define SRC_APP_UNI_ADC_H_

#include <uni/types.h>

#ifndef __adc_t_defined
typedef int adc_t;
#define __adc_t_defined 1
#endif

#ifndef __adc_val_t_defined
typedef unsigned long adc_val_t;
#define __adc_val_t_defined 1
#endif

void adc_init(adc_t t);
void adc_deinit(adc_t t);

void adc_on(adc_t t);
void adc_off(adc_t t);

adc_val_t adc_get(adc_t t);

#endif /* SRC_APP_UNI_ADC_H_ */
