/*
 * hal_dmai.c
 *
 *  Created on: 26 kwi 2018
 *      Author: kamil
 */

#include <string.h>
#include <minmax.h>
#include <machine/hal_dmafiforx.h>

void HAL_DMAfiforx_reset(struct HAL_DMAfiforx_s *t)
{
	assert(t);
	t->pos = 0;
}

size_t HAL_DMAfiforx_read8(struct HAL_DMAfiforx_s *t, uint32_t dma_cndtr,
		uint8_t out[], const size_t outlen)
{
	assert(t);
	assert(out);
	assert(t->data);
	assert(0 <= t->pos && t->pos <= t->size);

	const uint8_t * const data = t->data;
	const size_t head = t->pos;
	assert(dma_cndtr <= t->size);
	const size_t tail = t->size - dma_cndtr;
	const size_t end = t->size;
	assert(end > head && end > tail);
	if (head < tail) {
		const size_t toend = end - tail;
		if (outlen < toend) {
			memcpy(out, data+tail, outlen);
			t->pos += outlen;
			return outlen;
		} else {
			memcpy(out, data+tail, toend);
			const size_t left = outlen - toend;
			const size_t tocopy = MIN(left,head);
			memcpy(out+toend, data, tocopy);
			t->pos = tocopy;
			return tocopy+toend;
		}
	} else {
		const size_t tohead = tail - head;
		const size_t tocopy = MIN(tohead, outlen);
		memcpy(out, data+tail, tocopy);
		t->pos += tocopy;
		if (t->pos == end)
			t->pos = 0;
		return tocopy;
	}
}
