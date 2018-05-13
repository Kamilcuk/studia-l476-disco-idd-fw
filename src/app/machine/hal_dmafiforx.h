/*
 * hal_dmai.h
 *
 *  Created on: 26 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_APP_MACHINE_HAL_DMAFIFORX_H_
#define SRC_APP_MACHINE_HAL_DMAFIFORX_H_

#include <stdint.h>
#include <assert.h>
#include <stddef.h>

struct HAL_DMAfiforx_s {
	uint8_t * const data;
	const size_t size;
	size_t pos;
};

#define HAL_DMAFIFORX_INIT8(buf,size)   {(buf),(size)}
#define HAL_DMAFIFORX_INIT16(buf,size)  HAL_DMAFIFORX_INIT8((uint8_t*)(buf),sizeof(uint16_t)*(size))
#define HAL_DMAFIFORX_INIT32(buf,size)  HAL_DMAFIFORX_INIT8((uint8_t*)(buf),sizeof(uint32_t)*(size))

void HAL_DMAfiforx_reset(struct HAL_DMAfiforx_s *t);
size_t HAL_DMAfiforx_read8(struct HAL_DMAfiforx_s *t, uint32_t dma_cndtr,
		uint8_t out[], size_t outlen);

static inline uint8_t *HAL_DMAfiforx_getdata8(const struct HAL_DMAfiforx_s *t) {
	return t->data;
}

static inline uint16_t *HAL_DMAfiforx_getdata16(const struct HAL_DMAfiforx_s *t) {
	assert(t);
	assert(t->size%sizeof(uint16_t)==0);
	return (uint16_t*)t->data;
}

static inline uint32_t *HAL_DMAfiforx_getdata32(const struct HAL_DMAfiforx_s *t) {
	assert(t);
	assert(t->size%sizeof(uint32_t)==0);
	return (uint32_t*)t->data;
}

static inline size_t HAL_DMAfiforx_getsize8(const struct HAL_DMAfiforx_s *t) {
	assert(t);
	return t->size;
}
static inline size_t HAL_DMAfiforx_getsize16(const struct HAL_DMAfiforx_s *t) {
	assert(t);
	assert(t->size%sizeof(uint16_t)==0);
	return t->size/sizeof(uint16_t);
}
static inline size_t HAL_DMAfiforx_getsize32(const struct HAL_DMAfiforx_s *t) {
	assert(t);
	assert(t->size%sizeof(uint32_t)==0);
	return t->size/sizeof(uint32_t);
}

static inline
uint32_t HAL_DMAfiforx_read32(struct HAL_DMAfiforx_s *t, uint32_t dma_cndtr,
		uint32_t out[], size_t outlen)
{
	const uint32_t ret = HAL_DMAfiforx_read8(t, dma_cndtr*sizeof(out[0]),
			(uint8_t*)out, outlen*sizeof(out[0]))/sizeof(out[0]);
	assert(t->pos%sizeof(out[0]) == 0);
	return ret;
}


#endif /* SRC_APP_MACHINE_HAL_DMAFIFORX_H_ */
