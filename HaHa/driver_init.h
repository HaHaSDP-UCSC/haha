/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_timer.h>

#include <hal_usart_sync.h>
#include <hal_usart_async.h>

extern struct timer_descriptor TIMER_0;

extern struct usart_sync_descriptor  TARGET_IO;
extern struct usart_async_descriptor USART_1_0;

void TARGET_IO_PORT_init(void);
void TARGET_IO_CLOCK_init(void);
void TARGET_IO_init(void);

void USART_1_0_PORT_init(void);
void USART_1_0_CLOCK_init(void);
void USART_1_0_init(void);

/* ISR RAM table address, definition from rom code. */
#define ISR_RAM_MAP_START_ADDRESS (0x10000000)

/**
 * \brief ISR RAM table index
 *
 * Enum for the index of the ISR in RAm.
 *
 */
enum ram_isr_table_index {
	RAM_ISR_TABLE_RESET              = 1,
	RAM_ISR_TABLE_NMI                = 2,
	RAM_ISR_TABLE_HARDFAULT          = 3,
	RAM_ISR_TABLE_SVC                = 11,
	RAM_ISR_TABLE_PENDSV             = 14,
	RAM_ISR_TABLE_SYSTICK            = 15,
	RAM_ISR_TABLE_UARTRX0            = 16,
	RAM_ISR_TABLE_UARTTX0            = 17,
	RAM_ISR_TABLE_UARTRX1            = 18,
	RAM_ISR_TABLE_UARTTX1            = 19,
	RAM_ISR_TABLE_SPIRX0             = 20,
	RAM_ISR_TABLE_SPITX0             = 21,
	RAM_ISR_TABLE_SPIRX1             = 22,
	RAM_ISR_TABLE_SPITX1             = 23,
	RAM_ISR_TABLE_I2CRX0             = 24,
	RAM_ISR_TABLE_I2CTX0             = 25,
	RAM_ISR_TABLE_I2CRX1             = 26,
	RAM_ISR_TABLE_I2CTX1             = 27,
	RAM_ISR_TABLE_WDT0               = 28,
	RAM_ISR_TABLE_WDT1               = 29,
	RAM_ISR_TABLE_DUALTIMER          = 30,
	RAM_ISR_TABLE_BLE_PERIPH_REGS    = 31,
	RAM_ISR_TABLE_SECURITY           = 32,
	RAM_ISR_TABLE_EFUSE_OUT_OF_RESET = 33,
	RAM_ISR_TABLE_SPI_FLASH          = 34,
	RAM_ISR_TABLE_LP_CLK_CAL_DONE    = 35,
	RAM_ISR_TABLE_BROWNOUT_DETECTED  = 36,
	RAM_ISR_TABLE_BLE_CORE_RESET     = 37,
	RAM_ISR_TABLE_BLE_LP_CORE_RESET  = 38,
	RAM_ISR_TABLE_PORT0_COMB         = 39,
	RAM_ISR_TABLE_PORT1_COMB         = 40,
	RAM_ISR_TABLE_PORT2_COMB         = 41,
	RAM_ISR_TABLE_TIMER0             = 42,
	RAM_ISR_TABLE_AON_SLEEP_TIMER    = 43,
	RAM_ISR_TABLE_BLE_OSC_EN         = 44,
	RAM_ISR_TABLE_BLE_WAKEUP_LP      = 45,
	RAM_ISR_TABLE_RXTX_SEQ_SLEEP     = 46,
	RAM_ISR_TABLE_BLE_CORE           = 47,
};

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_H_INCLUDED
