/*
 * prif.h
 *
 *  Created on: 3 cze 2018
 *      Author: kamil
 *
 * Small header for printing float numbers on devices not supporting "%f" printf specifier
 * by calculating
 * (such as newlib nano)
 * Usage example:
 * double abc = 123.456;
 * printf("abc with 3 numbers after decimal=" PRIF(3) "\n"
 *        "abc with 6 numbers after decimal=" PRIF(6) "\n"
 *        ,
 *        PRIFARG(3, abc),
 *        PRIFARG(6, abc)
 * );
 */

#ifndef SRC_APP_PRIF_H_
#define SRC_APP_PRIF_H_

#define PRIF_RES_TO_DIV(res) \
	( \
		(res) == 0 ? 1 : \
		(res) == 1 ? 10 : \
		(res) == 2 ? 100 : \
		(res) == 3 ? 1000 : \
		(res) == 4 ? 10000 : \
		(res) == 5 ? 100000l : \
		(res) == 6 ? 1000000l : \
		(res) == 7 ? 10000000l : \
		(res) == 8 ? 100000000l : \
		(res) == 9 ? 1000000000l : \
		(res) \
	)

#define PRIF(res)  "%ld.%0" #res "ld"

#define PRIFARG_DIV(val, divisor)  \
		((long)(val)), ((long)((val) * (divisor))%(long)(divisor))

#define PRIFARG(res, val) \
		PRIFARG_DIV((val), PRIF_RES_TO_DIV(res))

/* more generic, use if you don't want to print "long" types */
#define PRIF_T(fmt, res)  "%" fmt ".%0" #res fmt
#define PRIFARG_DIV_T(printtype, val, divisor)  \
		((printtype)(val)), ((printtype)((val) * (divisor))%(printtype)(divisor))
#define PRIFARG_T(printtype, res, val) \
		PRIFARG_DIV_T(printtype, (val), PRIF_RES_TO_DIV(res))

#endif /* SRC_APP_PRIF_H_ */
