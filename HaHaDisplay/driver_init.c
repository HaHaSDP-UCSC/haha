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

extern struct _irq_descriptor *_irq_table[PERIPH_COUNT_IRQn];
extern void                    Default_Handler(void);

struct usart_sync_descriptor TARGET_IO;

void TARGET_IO_PORT_init(void)
{
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

void system_init(void)
{
	init_mcu();

	// GPIO on LP_GPIO_13

	// Set pin direction to output
	gpio_set_pin_direction(DISP_BRIGHT, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_BRIGHT,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_BRIGHT, GPIO_PIN_FUNCTION_OFF);

	// GPIO on LP_GPIO_14

	// Set pin direction to output
	gpio_set_pin_direction(DISP_B3, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_B3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_B3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on LP_GPIO_15

	// Set pin direction to output
	gpio_set_pin_direction(DISP_B2, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_B2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_B2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on LP_GPIO_16

	// Set pin direction to output
	gpio_set_pin_direction(DISP_B1, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_B1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_B1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on LP_GPIO_17

	// Set pin direction to output
	gpio_set_pin_direction(DISP_B0, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_B0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_B0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on LP_GPIO_18

	// Set pin direction to output
	gpio_set_pin_direction(DISP_E, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_E,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_E, GPIO_PIN_FUNCTION_OFF);

	// GPIO on LP_GPIO_19

	// Set pin direction to output
	gpio_set_pin_direction(DISP_REG, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_REG,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_REG, GPIO_PIN_FUNCTION_OFF);

	// GPIO on LP_GPIO_20

	// Set pin direction to output
	gpio_set_pin_direction(DISP_CTRST, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(DISP_CTRST,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(DISP_CTRST, GPIO_PIN_FUNCTION_OFF);

	UART0_register_isr();

	TARGET_IO_init();
}
