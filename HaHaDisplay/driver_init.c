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

void UART0_register_isr(void)
{
	uint32_t *temp;
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
	                   true);

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

	// GPIO on LP_GPIO_23

	// Set pin direction to input
	gpio_set_pin_direction(SW0, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SW0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(SW0, GPIO_PIN_FUNCTION_OFF);

	UART0_register_isr();

	TARGET_IO_init();
}
