/*
 * unistd.c
 *
 *  Created on: 25 kwi 2018
 *      Author: kamil
 */
#include <machine/mx.h>
#include <reent.h>
#include <unistd.h>
#include <assert.h>
#include <uni/pwrmode.h>
#include <machine/uart.h>
#include <ringbuffer.h>
#include <try.h>
#include <sys/types.h>

static const IRQn_Type huart3_irqs[] = {DMA1_Channel2_IRQn, DMA1_Channel3_IRQn, USART3_IRQn};
static char huart3_txbuf[16];
static struct HW_UART_dmatxrb_s huart3_uartdmatxrb = {
		.conf = {
				.huart = &huart3,
				.IRQlist = {huart3_irqs, __arraycount(huart3_irqs)},
		},
		.rb = RB_INIT(huart3_txbuf, __arraycount(huart3_txbuf)),
};

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	assert(huart == &huart3);
	HW_UART_dmatxrb_TxCplt_IRQHandler(&huart3_uartdmatxrb);
}

void io_fsync(int fd)
{
	switch(fd) {
	case STDOUT_FILENO:
		PWRMODE_WHILE(PWRMODE_SLEEP, !HAL_UART_IsTransmitReady(&huart3));
		break;
	case STDERR_FILENO:
		while(!HAL_UART_IsTransmitReady(&huart3));
		break;
	}
}

ssize_t io_write(int fd, const void *ptr, size_t nbyte)
{
	switch(fd) {
	case STDOUT_FILENO:
		while(nbyte) {
			const size_t written = HW_UART_dmatxrb_write(&huart3_uartdmatxrb, ptr, nbyte);
			if (written == 0) {
				io_fsync(fd);
			} else {
				nbyte -= written;
				ptr += written;
			}
		}
		break;
	case STDERR_FILENO:
		io_fsync(fd);
		try( HAL_UART_Transmit(&huart3, (uint8_t*)ptr, nbyte, UINT32_MAX) == HAL_OK );
		break;
	default:
		assert(0);
		break;
	}
	return nbyte;
}

ssize_t _write_r(struct _reent *r, int fd, const void *ptr, size_t nbyte)
{
	return io_write(fd, ptr, nbyte);
}

int fsync(int fd)
{
	return io_fsync(fd), 0;
}
