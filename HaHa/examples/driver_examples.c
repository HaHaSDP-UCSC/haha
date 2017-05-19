/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using TIMER_0.
 */
static struct timer_task TIMER_0_task;

static void TIMER_0_wakeup_task_cb(const struct timer_task *const timer_task)
{
}

void TIMER_0_example(void)
{
	TIMER_0_task.interval = 1;
	TIMER_0_task.cb       = TIMER_0_wakeup_task_cb;
	TIMER_0_task.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task);
	timer_start(&TIMER_0);
}

/**
 * Example of using PWM_0.
 */
void PWM_0_example(void)
{
	pwm_sync_set_parameters(&PWM_0, 4, 50);
	pwm_sync_enable(&PWM_0);
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using USART_1_0 to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_USART_1_0[12] = "Hello World!";

static void tx_cb_USART_1_0(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void USART_1_0_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&USART_1_0, USART_ASYNC_TXC_CB, tx_cb_USART_1_0);
	/*usart_async_register_callback(&USART_1_0, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&USART_1_0, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&USART_1_0, &io);
	usart_async_enable(&USART_1_0);

	io_write(io, example_USART_1_0, 12);
}
