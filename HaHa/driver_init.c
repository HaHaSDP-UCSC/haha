/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <utils.h>
#include <hal_init.h>

#include <hpl_dualtimer.h>

/*! The buffer size for USART */
#define USART_1_0_BUFFER_SIZE 16

struct timer_descriptor       TIMER_0;
struct usart_async_descriptor USART_1_0;

static uint8_t USART_1_0_buffer[USART_1_0_BUFFER_SIZE];

struct usart_sync_descriptor TARGET_IO;

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_0_init(void)
{
	timer_init(&TIMER_0, (uint8_t *)DUALTIMER0 + 0 * 0x20, _dt_get_timer());
}

void EXTERNAL_IRQ_0_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(LP_GPIO_23, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(LP_GPIO_23,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(LP_GPIO_23, GPIO_PIN_FUNCTION_OFF);
}

void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(LP_GPIO_2, PINMUX_LP_GPIO_2_M_UART0_RXD);

	gpio_set_pin_function(LP_GPIO_3, PINMUX_LP_GPIO_3_M_UART0_TXD);
}

void TARGET_IO_CLOCK_init(void)
{
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	usart_sync_init(&TARGET_IO, UART0, (void *)NULL);
	TARGET_IO_PORT_init();
}

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void USART_1_0_CLOCK_init()
{
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void USART_1_0_PORT_init()
{

	gpio_set_pin_function(LP_GPIO_14, PINMUX_LP_GPIO_14_M_UART1_CTS);

	gpio_set_pin_function(LP_GPIO_15, PINMUX_LP_GPIO_15_M_UART1_RTS);

	gpio_set_pin_function(LP_GPIO_6, PINMUX_LP_GPIO_6_M_UART1_RXD);

	gpio_set_pin_function(LP_GPIO_7, PINMUX_LP_GPIO_7_M_UART1_TXD);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void USART_1_0_init(void)
{
	USART_1_0_CLOCK_init();
	usart_async_init(&USART_1_0, UART1, USART_1_0_buffer, USART_1_0_BUFFER_SIZE, (void *)NULL);
	USART_1_0_PORT_init();
}

void DUALTIMER0_register_isr(void)
{
	uint32_t *temp;

	temp  = (uint32_t *)(RAM_ISR_TABLE_DUALTIMER * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)DUALTIMER0_Handler;
}

void GPIO1_register_isr(void)
{
	uint32_t *temp;

	temp  = (uint32_t *)((RAM_ISR_TABLE_PORT0_COMB + 1) * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)GPIO1_Handler;
}

void UART0_register_isr(void)
{
	uint32_t *temp;
}

void UART1_register_isr(void)
{
	uint32_t *temp;

	temp  = (uint32_t *)((RAM_ISR_TABLE_UARTRX0 + (1 << 1)) * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)UART1_RX_Handler;

	temp  = (uint32_t *)((RAM_ISR_TABLE_UARTTX0 + (1 << 1)) * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)UART1_TX_Handler;
}

void system_init(void)
{
	init_mcu();

	DUALTIMER0_register_isr();

	TIMER_0_init();

	GPIO1_register_isr();
	EXTERNAL_IRQ_0_init();

	UART0_register_isr();

	TARGET_IO_init();

	UART1_register_isr();
	USART_1_0_init();

	ext_irq_init();
}
