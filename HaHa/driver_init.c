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
#include <hpl_irq.h>

#if CONF_DMAC_MAX_USED_DESC > 0
#include <hpl_dma.h>
#include <hpl_prov_dma_ctrl_v100.h>

COMPILER_ALIGNED(16)
DmacDescriptor _descriptor_section[CONF_DMAC_MAX_USED_DESC] SECTION_DMAC_DESCRIPTOR;

struct _dma_resource _resource[CONF_DMAC_MAX_USED_CH];

uint32_t dmac_ch_used = CONF_DMAC_MAX_USED_CH;
#endif

/*! The buffer size for USART */
#define USART_1_0_BUFFER_SIZE 16

extern struct _irq_descriptor *_irq_table[PERIPH_COUNT_IRQn];
extern void                    Default_Handler(void);

struct usart_async_descriptor USART_1_0;

static uint8_t USART_1_0_buffer[USART_1_0_BUFFER_SIZE];

struct usart_sync_descriptor TARGET_IO;

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

void UART0_RX_Handler(void)
{
	if (_irq_table[UART0_RX_IRQn + (0 << 1)]) {
		_irq_table[UART0_RX_IRQn + (0 << 1)]->handler(_irq_table[UART0_RX_IRQn + (0 << 1)]->parameter);
	} else {
		Default_Handler();
	}
}

void UART0_TX_Handler(void)
{
	if (_irq_table[UART0_TX_IRQn + (0 << 1)]) {
		_irq_table[UART0_TX_IRQn + (0 << 1)]->handler(_irq_table[UART0_TX_IRQn + (0 << 1)]->parameter);
	} else {
		Default_Handler();
	}
}
void UART0_register_isr(void)
{
	uint32_t *temp;

	temp  = (uint32_t *)((RAM_ISR_TABLE_UARTRX0 + (0 << 1)) * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)UART0_RX_Handler;

	temp  = (uint32_t *)((RAM_ISR_TABLE_UARTTX0 + (0 << 1)) * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)UART0_TX_Handler;
}

void UART1_RX_Handler(void)
{
	if (_irq_table[UART0_RX_IRQn + (1 << 1)]) {
		_irq_table[UART0_RX_IRQn + (1 << 1)]->handler(_irq_table[UART0_RX_IRQn + (1 << 1)]->parameter);
	} else {
		Default_Handler();
	}
}

void UART1_TX_Handler(void)
{
	if (_irq_table[UART0_TX_IRQn + (1 << 1)]) {
		_irq_table[UART0_TX_IRQn + (1 << 1)]->handler(_irq_table[UART0_TX_IRQn + (1 << 1)]->parameter);
	} else {
		Default_Handler();
	}
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

	UART0_register_isr();

	TARGET_IO_init();

	UART1_register_isr();
	USART_1_0_init();
}
