/**
 * \file
 *
 * \brief SAM Serial Communication Interface
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <utils_assert.h>
#include <utils.h>
#include <hpl_usart_async.h>
#include <hpl_usart_sync.h>
#include <hpl_uart_config.h>
#include <stdio.h>
#include "ble_config.h"
#ifndef CONF_UART_0_ENABLE
#define CONF_UART_0_ENABLE 0
#endif
#ifndef CONF_UART_1_ENABLE
#define CONF_UART_1_ENABLE 0
#endif

uart_rx_callback_t user_callback_func = NULL;

extern struct _usart_async_device SAMB11_IO;
/** Amount of UART. */
#define UART_AMOUNT (CONF_UART_0_ENABLE + CONF_UART_1_ENABLE)

/**
 * \brief Macro is used to fill UART configuration structure based on
 * its number
 *
 * \param[in] n The number of structures
 */
#define UART_CONFIGURATION(n)                                                                                          \
	{                                                                                                                  \
		(n), CONF_UART_##n##_BAUD_RATE, CONF_UART_##n##_CHSIZE, CONF_UART_##n##_SBMODE, CONF_UART_##n##_PARITY,        \
		    CONF_UART_##n##_FLOW_CONTROL, CONF_UART_##n##_CLOCK_PRESCALER, CONF_UART_##n##_CLOCK_FREQUENCY,            \
	}

/**
 * \brief Configuration structure for the UART module
 *
 * This is the configuration structure for the UART Module in SAMB11. It
 * is used as an argument for \ref uart_init to provide the desired
 * configurations for the module. The structure should be initialized using the
 * \ref uart_get_config_defaults .
 */
struct uart_config {
	uint8_t  number;
	uint32_t baud_rate;       /** Baud rate */
	uint8_t  data_bits;       /** Number of data bits */
	uint8_t  stop_bits;       /** Number of stop bits */
	uint8_t  parity;          /** Parity type */
	uint8_t  flow_control;    /** flow control type */
	uint8_t  prescaler;       /** source clock frequency prescaler */
	uint32_t clock_frequence; /** source clock frequency */
};

#if UART_AMOUNT < 1
/** Dummy array to pass compiling. */
static struct uart_config _uart_config[1] = {{0}};
#else
/**
 * \brief Array of UART configurations
 */
static struct uart_config _uart_config[] = {
#if CONF_UART_0_ENABLE == 1
    UART_CONFIGURATION(0),
#endif
#if CONF_UART_1_ENABLE == 1
    UART_CONFIGURATION(1),
#endif
};
#endif

static void _uart_interrupt_handler(void *p);
static uint8_t _get_uart_index(const void *const hw);
static uint8_t _uart_get_irq_num(const void *const hw);
static void _uart_set_baud_rate(void *const hw, const uint32_t baud_rate);

/**
 * \brief Retrieve ordinal number of the given UART hardware instance
 */
static uint8_t _get_uart_index(const void *const hw)
{
	ASSERT(hw);

	return ((uint32_t)hw - (uint32_t)UART0) >> 12;
}

/**
 * \internal Retrieve IRQ number of the given UART hardware instance
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return The IRQ index of hardware instance
 */
static uint8_t _uart_get_irq_num(const void *const hw)
{
	ASSERT(hw);

	return ((((uint32_t)hw - (uint32_t)UART0) >> 12) << 1) + UART0_RX_IRQn;
}

/**
 * \internal Sercom interrupt handler
 *
 * \param[in] p The pointer to interrupt parameter
 */
static void _uart_interrupt_handler(void *p)
{
	ASSERT(p);

	struct _usart_async_device *device = (struct _usart_async_device *)p;
	void *                      hw     = device->hw;
	
	if (hri_uart_get_TX_INTERRUPT_MASK_TX_FIFO_NOT_FULL_MASK_bit(hw) && hri_uart_get_TRANSMIT_STATUS_TX_FIFO_NOT_FULL_bit(hw)) {
		device->usart_cb.tx_byte_sent(device);
	} else if (hri_uart_get_TX_INTERRUPT_MASK_TX_FIFO_EMPTY_MASK_bit(hw) && hri_uart_get_TRANSMIT_STATUS_TX_FIFO_EMPTY_bit(hw)) {
		hri_uart_clear_TX_INTERRUPT_MASK_TX_FIFO_EMPTY_MASK_bit(device->hw);

	} else if (hri_uart_get_RX_INTERRUPT_MASK_RX_FIFO_NOT_EMPTY_MASK_bit(hw) && hri_uart_get_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY_bit(hw)) {
		user_callback_func(hri_uart_read_RECEIVE_DATA_reg(hw));
		//device->usart_cb.rx_done_cb(device, hri_uart_read_RECEIVE_DATA_reg(hw));
		
		//hri_uart_write_RX_INTERRUPT_MASK_reg(hw, 0);
	} else if (hri_uart_get_RX_INTERRUPT_MASK_TIMEOUT_MASK_bit(hw) && hri_uart_get_RECEIVE_STATUS_TIMEOUT_bit(hw)) {
		device->usart_cb.error_cb(device);
	} else if (hri_uart_get_RX_INTERRUPT_MASK_PARITY_ERROR_MASK_bit(hw) && hri_uart_get_RECEIVE_STATUS_PARITY_ERROR_bit(hw)) {
		device->usart_cb.error_cb(device);
	} else if (hri_uart_get_RX_INTERRUPT_MASK_FIFO_OVERRUN_MASK_bit(hw) && hri_uart_get_RECEIVE_STATUS_FIFO_OVERRUN_bit(hw)) {
		device->usart_cb.error_cb(device);
	} else if (hri_uart_get_RX_INTERRUPT_MASK_FRAMING_ERROR_MASK_bit(hw) && hri_uart_get_RECEIVE_STATUS_FRAMING_ERROR_bit(hw)) {
		device->usart_cb.error_cb(device);
	}
}

/**
 * \internal Initialize UART
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return The status of initialization
 */
static int32_t _uart_init(void *const hw)
{
	/* Sanity check arguments */
	ASSERT(hw);

	uint8_t i = _get_uart_index(hw);

	if (hw == UART0) {
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART0_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART0_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART0_IF_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART0_IF_RSTN_bit(LPMCU_MISC_REGS0);
	} else if (hw == UART1) {
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART1_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART1_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART1_IF_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART1_IF_RSTN_bit(LPMCU_MISC_REGS0);
	}

	/* empty UART FIFO */
	while (hri_uart_get_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY_bit(hw)) {
		i = hri_uart_read_RECEIVE_DATA_reg(hw);
	}

	/* reset configuration register */
	hri_uart_write_CONFIGURATION_reg(hw, 0);

	/* program the uart configuration. */
	if (_uart_config[i].flow_control) {
		hri_uart_set_CONFIGURATION_CTS_ENABLE_bit(hw);
	}
	if (_uart_config[i].data_bits) {
		hri_uart_set_CONFIGURATION_NUMBER_OF_BITS_bit(hw);
	}
	if (_uart_config[i].stop_bits) {
		hri_uart_set_CONFIGURATION_STOP_BITS_bit(hw);
	}
	switch (_uart_config[i].parity) {
	case USART_PARITY_NONE:
		hri_uart_clear_CONFIGURATION_PARITY_ENABLE_bit(hw);
		break;

	case USART_PARITY_EVEN:
		hri_uart_set_CONFIGURATION_PARITY_ENABLE_bit(hw);
		hri_uart_write_CONFIGURATION_PARITY_MODE_bf(hw, _uart_config[i].parity);
		break;

	case USART_PARITY_ODD:
		hri_uart_set_CONFIGURATION_PARITY_ENABLE_bit(hw);
		hri_uart_write_CONFIGURATION_PARITY_MODE_bf(hw, _uart_config[i].parity);
		break;

	default:
		break;
	}

	/* Calculate the baud rate. */
	hri_uart_write_CLOCK_SOURCE_reg(hw, _uart_config[i].prescaler);
	_uart_set_baud_rate(hw, (_uart_config[i].baud_rate));

	hri_uart_write_RX_INTERRUPT_MASK_reg(hw, 0);
	hri_uart_write_TX_INTERRUPT_MASK_reg(hw, 0);

	return ERR_NONE;
}

/**
 * \internal De-initialize UART
 *
 * \param[in] hw The pointer to hardware instance
 */
static inline void _uart_deinit(void *const hw)
{
	ASSERT(hw);

	if (hw == UART0) {
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART0_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART0_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART0_IF_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART0_IF_RSTN_bit(LPMCU_MISC_REGS0);
	} else if (hw == UART1) {
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART1_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART1_CORE_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_0_UART1_IF_RSTN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_0_UART1_IF_RSTN_bit(LPMCU_MISC_REGS0);
	}
}

/**
 * \internal Set baud rate
 *
 * \param[in] device The pointer to UART device instance
 * \param[in] baud_rate A baud rate to set
 */
static void _uart_set_baud_rate(void *const hw, const uint32_t baud_rate)
{
	ASSERT(hw);

	uint32_t clock;
	uint16_t integerpart    = 0;
	uint8_t  fractionalpart = 0;
	uint32_t diff;
	uint8_t  i = 0;
	uint8_t  j = _get_uart_index(hw);

	clock       = _uart_config[j].clock_frequence;
	integerpart = clock / baud_rate;
	diff        = clock - (baud_rate * integerpart);
	i           = 0;
	while (diff > (baud_rate >> 4)) {
		i++;
		diff -= (baud_rate >> 4);
	}
	fractionalpart = (i + 1) / 2;

	hri_uart_write_BAUD_RATE_INTEGER_DIVISION_bf(hw, integerpart);
	hri_uart_write_BAUD_RATE_FRACTIONAL_DIVISION_bf(hw, fractionalpart);
}

/**
 * \internal Set parity
 *
 * \param[in] device The pointer to UART device instance
 * \param[in] parity A parity to set
 */
static void _uart_set_parity(void *const hw, const enum usart_parity parity)
{
	uint8_t i = _get_uart_index(hw);

	ASSERT(hw);

	switch (parity) {
	case USART_PARITY_NONE:
		hri_uart_clear_CONFIGURATION_PARITY_ENABLE_bit(hw);
		break;

	case USART_PARITY_EVEN:
		hri_uart_set_CONFIGURATION_PARITY_ENABLE_bit(hw);
		hri_uart_write_CONFIGURATION_PARITY_MODE_bf(hw, _uart_config[i].parity);
		break;

	case USART_PARITY_ODD:
		hri_uart_set_CONFIGURATION_PARITY_ENABLE_bit(hw);
		hri_uart_write_CONFIGURATION_PARITY_MODE_bf(hw, _uart_config[i].parity);
		break;

	default:
		break;
	}
}

/**
 * \internal Set stop bits mode
 *
 * \param[in] device The pointer to UART device instance
 * \param[in] stop_bits A stop bits mode to set
 */
static void _uart_set_stop_bits(void *const hw, const enum usart_stop_bits stop_bits)
{
	ASSERT(hw);

	hri_uart_write_CONFIGURATION_STOP_BITS_bit(hw, stop_bits);
}

/**
 * \internal Set character size
 *
 * \param[in] device The pointer to UART device instance
 * \param[in] size A character size to set
 */
static void _uart_set_character_size(void *const hw, const enum usart_character_size size)
{
	ASSERT(hw);

	if (size == USART_CHARACTER_SIZE_8BITS) {
		hri_uart_write_CONFIGURATION_NUMBER_OF_BITS_bit(hw, 0);
	} else {
		hri_uart_write_CONFIGURATION_NUMBER_OF_BITS_bit(hw, 1);
	}
}

/**
 * \brief Initialize synchronous UART
 */
int32_t _usart_sync_init(struct _usart_sync_device *const device, void *const hw)
{
	ASSERT(device && hw);

	device->hw = hw;

	return _uart_init(hw);
}

/**
 * \brief Initialize asynchronous UART
 */
int32_t _usart_async_init(struct _usart_async_device *const device, void *const hw)
{
	int32_t init_status;

	ASSERT(device && hw);

	init_status = _uart_init(hw);
	if (init_status) {
		return init_status;
	}
	device->hw = hw;

	device->irq.handler   = _uart_interrupt_handler;
	device->irq.parameter = (void *)device;

	/* Disable RX and TX interrupt */
	_irq_disable((IRQn_Type)_uart_get_irq_num(hw));
	_irq_disable((IRQn_Type)_uart_get_irq_num(hw) + 1);
	/* Clear RX and TX interrupt */
	_irq_clear((IRQn_Type)_uart_get_irq_num(hw));
	_irq_clear((IRQn_Type)_uart_get_irq_num(hw) + 1);

	_irq_register(_uart_get_irq_num(hw), &device->irq);
	_irq_register(_uart_get_irq_num(hw) + 1, &device->irq);
	_irq_enable((IRQn_Type)_uart_get_irq_num(hw));
	_irq_enable((IRQn_Type)_uart_get_irq_num(hw) + 1);

	return ERR_NONE;
}

/**
 * \brief De-initialize UART
 */
void _usart_sync_deinit(struct _usart_sync_device *const device)
{
	ASSERT(device);

	_uart_deinit(device->hw);
}

/**
 * \brief De-initialize UART
 */
void _usart_async_deinit(struct _usart_async_device *const device)
{
	ASSERT(device);

	_irq_disable((IRQn_Type)_uart_get_irq_num(device->hw));
	_uart_deinit(device->hw);
}

/**
 * \brief Enable UART module
 */
void _usart_sync_enable(struct _usart_sync_device *const device)
{
	ASSERT(device);
	(void)device;
}

/**
 * \brief Enable UART module
 */
void _usart_async_enable(struct _usart_async_device *const device)
{
	ASSERT(device);
	(void)device;
}

/**
 * \brief Disable UART module
 */
void _usart_sync_disable(struct _usart_sync_device *const device)
{
	ASSERT(device);
	(void)device;
}

/**
 * \brief Disable UART module
 */
void _usart_async_disable(struct _usart_async_device *const device)
{
	ASSERT(device);
	(void)device;
}

/**
 * \brief Set baud rate
 */
void _usart_sync_set_baud_rate(struct _usart_sync_device *const device, const uint32_t baud_rate)
{
	ASSERT(device);

	_uart_set_baud_rate(device->hw, baud_rate);
}

/**
 * \brief Set baud rate
 */
void _usart_async_set_baud_rate(struct _usart_async_device *const device, const uint32_t baud_rate)
{
	ASSERT(device);

	_uart_set_baud_rate(device->hw, baud_rate);
}

/**
 * \brief Set data order
 */
void _usart_sync_set_data_order(struct _usart_sync_device *const device, const enum usart_data_order order)
{
	ASSERT(device);
	(void)device;
	(void)order;
}

/**
 * \brief Set data order
 */
void _usart_async_set_data_order(struct _usart_async_device *const device, const enum usart_data_order order)
{
	ASSERT(device);
	(void)device;
	(void)order;
}

/**
 * \brief Set mode
 */
void _usart_sync_set_mode(struct _usart_sync_device *const device, const enum usart_mode mode)
{
	ASSERT(device && device->hw);
	(void)device;
	(void)mode;
}

/**
 * \brief Set parity
 */
void _usart_sync_set_parity(struct _usart_sync_device *const device, const enum usart_parity parity)
{
	ASSERT(device);

	_uart_set_parity(device->hw, parity);
}

/**
 * \brief Set parity
 */
void _usart_async_set_parity(struct _usart_async_device *const device, const enum usart_parity parity)
{
	ASSERT(device);

	_uart_set_parity(device->hw, parity);
}

/**
 * \brief Set stop bits mode
 */
void _usart_sync_set_stop_bits(struct _usart_sync_device *const device, const enum usart_stop_bits stop_bits)
{
	ASSERT(device);

	_uart_set_stop_bits(device->hw, stop_bits);
}

/**
 * \brief Set stop bits mode
 */
void _usart_async_set_stop_bits(struct _usart_async_device *const device, const enum usart_stop_bits stop_bits)
{
	ASSERT(device);

	_uart_set_stop_bits(device->hw, stop_bits);
}

/**
 * \brief Set character size
 */
void _usart_sync_set_character_size(struct _usart_sync_device *const device, const enum usart_character_size size)
{
	ASSERT(device);

	_uart_set_character_size(device->hw, size);
}

/**
 * \brief Set character size
 */
void _usart_async_set_character_size(struct _usart_async_device *const device, const enum usart_character_size size)
{
	ASSERT(device);

	_uart_set_character_size(device->hw, size);
}

/**
 * \brief Retrieve uart status
 */
uint32_t _usart_sync_get_status(const struct _usart_sync_device *const device)
{
	ASSERT(device);

	return hri_uart_read_TRANSMIT_STATUS_reg(device->hw) | (hri_uart_read_RECEIVE_STATUS_reg(device->hw) << 8);
}

/**
 * \brief Retrieve uart status
 */
uint32_t _usart_async_get_status(const struct _usart_async_device *const device)
{
	ASSERT(device);
	
	return hri_uart_read_TRANSMIT_STATUS_reg(device->hw) | (hri_uart_read_RECEIVE_STATUS_reg(device->hw) << 8);
}

/**
 * \brief Write a byte to the given UART instance
 */
void _usart_sync_write_byte(struct _usart_sync_device *const device, uint8_t data)
{
	ASSERT(device);

	hri_uart_write_TRANSMIT_DATA_reg(device->hw, data);
}

/**
 * \brief Write a byte to the given UART instance
 */
void _usart_async_write_byte(struct _usart_async_device *const device, uint8_t data)
{
	ASSERT(device);

	hri_uart_write_TRANSMIT_DATA_reg(device->hw, data);
}

/**
 * \brief Read a byte from the given UART instance
 */
uint8_t _usart_sync_read_byte(const struct _usart_sync_device *const device)
{
	ASSERT(device);

	return hri_uart_read_RECEIVE_DATA_reg(device->hw);
}

/**
 * \brief Read a byte from the given UART instance
 */
uint8_t _usart_async_read_byte(const struct _usart_async_device *const device)
{
	ASSERT(device);

	return hri_uart_read_RECEIVE_DATA_reg(device->hw);
}

/**
 * \brief Check if UART is ready to send next byte
 */
bool _usart_sync_is_byte_sent(const struct _usart_sync_device *const device)
{
	ASSERT(device);

	return hri_uart_get_TRANSMIT_STATUS_TX_FIFO_EMPTY_bit(device->hw);
}

/**
 * \brief Check if UART is ready to send next byte
 */
bool _usart_async_is_byte_sent(const struct _usart_async_device *const device)
{
	ASSERT(device);

	return hri_uart_get_TRANSMIT_STATUS_TX_FIFO_EMPTY_bit(device->hw);
}

/**
 * \brief Check if there is data received by UART
 */
bool _usart_sync_is_byte_received(const struct _usart_sync_device *const device)
{
	ASSERT(device);

	return hri_uart_get_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY_bit(device->hw);
}

/**
 * \brief Check if there is data received by UART
 */
bool _usart_async_is_byte_received(const struct _usart_async_device *const device)
{
	ASSERT(device);

	return hri_uart_get_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY_bit(device->hw);
}

/**
 * \brief Set the state of flow control pins
 */
void _usart_sync_set_flow_control_state(struct _usart_sync_device *const     device,
                                        const union usart_flow_control_state state)
{
	ASSERT(device);

	(void)device;
	(void)state;
}

/**
 * \brief Set the state of flow control pins
 */
void _usart_async_set_flow_control_state(struct _usart_async_device *const    device,
                                         const union usart_flow_control_state state)
{
	ASSERT(device);

	(void)device;
	(void)state;
}

/**
 * \brief Retrieve the state of flow control pins
 */
union usart_flow_control_state _usart_sync_get_flow_control_state(const struct _usart_sync_device *const device)
{
	ASSERT(device);

	(void)device;
	union usart_flow_control_state state;

	state.value           = 0;
	state.bit.unavailable = 1;
	return state;
}

/**
 * \brief Retrieve the state of flow control pins
 */
union usart_flow_control_state _usart_async_get_flow_control_state(const struct _usart_async_device *const device)
{
	ASSERT(device);

	(void)device;
	union usart_flow_control_state state;

	state.value           = 0;
	state.bit.unavailable = 1;
	return state;
}

/**
 * \brief Enable data register empty interrupt
 */
void _usart_async_enable_byte_sent_irq(struct _usart_async_device *const device)
{
	ASSERT(device);

	hri_uart_set_TX_INTERRUPT_MASK_TX_FIFO_NOT_FULL_MASK_bit(device->hw);
}

/**
 * \brief Enable transmission complete interrupt
 */
void _usart_async_enable_tx_done_irq(struct _usart_async_device *const device)
{
	ASSERT(device);

	hri_uart_clear_TX_INTERRUPT_MASK_TX_FIFO_NOT_FULL_MASK_bit(device->hw);
	hri_uart_set_TX_INTERRUPT_MASK_TX_FIFO_EMPTY_MASK_bit(device->hw);
}

/**
 * \brief Enable/disable UART interrupt
 *
 * param[in] device The pointer to UART device instance
 * param[in] type The type of interrupt to disable/enable if applicable
 * param[in] state Enable or disable
 */
void _usart_async_set_irq_state(struct _usart_async_device *const device, const enum _usart_async_callback_type type,
                                const bool state)
{
	ASSERT(device);

	if ((USART_ASYNC_BYTE_SENT == type) || (USART_ASYNC_TX_DONE == type)) {
		hri_uart_write_TX_INTERRUPT_MASK_TX_FIFO_NOT_FULL_MASK_bit(device->hw, state);
	} else if (USART_ASYNC_RX_DONE == type) {
		hri_uart_write_RX_INTERRUPT_MASK_RX_FIFO_NOT_EMPTY_MASK_bit(device->hw, state);
	} else if (USART_ASYNC_ERROR == type) {
		hri_uart_write_RX_INTERRUPT_MASK_TIMEOUT_MASK_bit(device->hw, state);
		hri_uart_write_RX_INTERRUPT_MASK_PARITY_ERROR_MASK_bit(device->hw, state);
		hri_uart_write_RX_INTERRUPT_MASK_FIFO_OVERRUN_MASK_bit(device->hw, state);
		hri_uart_write_RX_INTERRUPT_MASK_FRAMING_ERROR_MASK_bit(device->hw, state);
	}
}

void _usart_async_enable_byte_received_irq(struct _usart_async_device *const device)
{
	ASSERT(device);

	hri_uart_set_RX_INTERRUPT_MASK_RX_FIFO_NOT_EMPTY_MASK_bit(device->hw);
}

void register_uart_callback(uart_rx_callback_t callback_func)
{
	user_callback_func = callback_func;
	//uart_enable_callback(&uart_instance, UART_RX_COMPLETE);
	_usart_async_read_byte(&CONSOLE_IO);
	_usart_async_enable_byte_received_irq(&CONSOLE_IO);
}
