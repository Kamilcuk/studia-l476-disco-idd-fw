/*
 * hal_dmai.c
 *
 *  Created on: 26 kwi 2018
 *      Author: kamil
 */

#include <machine/hal_dmafiforx.h>
#include <string.h>
#include <minmax.h>
#include <stdio.h>

//#define DBG(...)  printf(__VA_ARGS__)
#define DBG(...)  do{}while(0)

void HAL_DMAfiforx_reset(struct HAL_DMAfiforx_s *t)
{
	assert(t);
	t->pos = 0;
}

size_t HAL_DMAfiforx_read8(struct HAL_DMAfiforx_s *t, uint32_t dma_cndtr,
		uint8_t out[], const size_t outlen)
{
	static_assert(sizeof(uint8_t) == sizeof(char),"");
	assert(t);
	assert(out);
	assert(t->data);
	assert(0 <= t->pos && t->pos < t->size);

	const uint8_t * const data = t->data;
	const size_t head = t->pos;
	assert(dma_cndtr <= t->size);
	const size_t tail = t->size - dma_cndtr;
	const size_t size = t->size;
	size_t ret;
	assert(size > head && size > tail);
	if (head < tail) {
		const size_t toend = size - tail;
		if (outlen < toend) {
			memcpy(out, data+tail, outlen);
			DBG("1 %lu %lu %lu %lu %lu\n", t->pos, dma_cndtr, outlen, size, tail);
			t->pos += outlen;
			if (t->pos == size)
				t->pos = 0;
			ret = outlen;
		} else {
			memcpy(out, data+tail, toend);
			const size_t left = outlen - toend;
			const size_t tocopy = MIN(left,head);
			memcpy(out+toend, data, tocopy);
			DBG("2 %lu %lu %lu %lu %lu %lu\n", t->pos, dma_cndtr, tocopy, toend, outlen, left);
			t->pos = tocopy;
			assert(0 <= t->pos && t->pos < t->size);
			ret = tocopy+toend;
		}
	} else {
		const size_t tohead = tail - head;
		const size_t tocopy = MIN(tohead, outlen);
		memcpy(out, data+tail, tocopy);
		DBG("3 %lu %lu %lu %lu %lu\n", t->pos, dma_cndtr, tocopy, tail, head);
		t->pos += tocopy;
		if (t->pos == size)
			t->pos = 0;
		ret = tocopy;
	}
	assert(0 <= t->pos && t->pos < t->size);
	return ret;
}
