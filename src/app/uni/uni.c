/*
 * uni.c
 *
 *  Created on: 25 kwi 2018
 *      Author: kamil
 */
#include <uni/uni.h>
#include <uni/priv.h>
#include <uni/wdg.h>
#include <machine/mx.h>

void uni_init(void)
{
	uni_wdg_init();
	MX_Init();
	__HAL_RCC_CLEAR_RESET_FLAGS();
}
