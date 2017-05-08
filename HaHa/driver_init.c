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
#include <stdio.h>
#include <hpl_dualtimer.h>
#include "hpl_usart_async.h"
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

struct timer_descriptor TIMER_0;

struct usart_sync_descriptor TARGET_IO;

struct _usart_async_device SAMB11_IO;
/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */

void TIMER_0_init(void)
{
	puts("Init Timer\r\n");
	timer_init(&TIMER_0, (uint8_t *)DUALTIMER0 + 0 * 0x20, _dt_get_timer());
}
void EXTERNAL_IRQ_0_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(BLE_APP_SW, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(BLE_APP_SW,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(BLE_APP_SW, GPIO_PIN_FUNCTION_OFF);
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

void DUALTIMER0_Handler(void)
{
	if (_irq_table[DUALTIMER0_IRQn]) {
		_irq_table[DUALTIMER0_IRQn]->handler(_irq_table[DUALTIMER0_IRQn]->parameter);
	} else {
		Default_Handler();
	}
}

void DUALTIMER0_register_isr(void)
{
	uint32_t *temp;

	temp  = (uint32_t *)(RAM_ISR_TABLE_DUALTIMER * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)DUALTIMER0_Handler;
}

void GPIO1_Handler(void)
{
	if (_irq_table[GPIO0_IRQn + 1]) {
		_irq_table[GPIO0_IRQn + 1]->handler(_irq_table[GPIO0_IRQn + 1]->parameter);
	} else {
		Default_Handler();
	}
}
void GPIO1_register_isr(void)
{
	uint32_t *temp;

	temp  = (uint32_t *)((RAM_ISR_TABLE_PORT0_COMB + 1) * 4 + ISR_RAM_MAP_START_ADDRESS);
	*temp = (uint32_t)GPIO1_Handler;
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

	// GPIO on LP_GPIO_22

	// Set pin direction to output
	gpio_set_pin_direction(BLE_APP_LED, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(BLE_APP_LED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(BLE_APP_LED, GPIO_PIN_FUNCTION_OFF);

	DUALTIMER0_register_isr();

	//TIMER_0_init();

	GPIO1_register_isr();
	EXTERNAL_IRQ_0_init();

	UART0_register_isr();

	TARGET_IO_init();

	ext_irq_init();
}
