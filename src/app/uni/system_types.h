/*
 * machine_types.h
 *
 *  Created on: 25 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_APP_UNI_SYSTEM_TYPES_H_
#define SRC_APP_UNI_SYSTEM_TYPES_H_

#include <uni/types.h>
#include <machine/mx.h>

typedef struct {
	double vref;
	double val;
} adc_val_t;
#define __adc_val_t_defined 1

static const gpio_t gpio_mfx_wakeup = GPIO_INIT(MFX_WAKEUP_GPIO_Port, MFX_WAKEUP_Pin);
static const gpio_t gpio_cal = GPIO_INIT(CAL_GPIO_Port, CAL_Pin);
static const gpio_t gpio_sh0 = GPIO_INIT(SH0_GPIO_Port, SH0_Pin);
static const gpio_t gpio_sh1 = GPIO_INIT(SH1_GPIO_Port, SH1_Pin);
static const gpio_t gpio_sh2 = GPIO_INIT(SH2_GPIO_Port, SH2_Pin);
static const gpio_t gpio_idd_meas = GPIO_INIT(IDD_MEAS_GPIO_Port, IDD_MEAS_Pin);

#endif /* SRC_APP_UNI_SYSTEM_TYPES_H_ */
