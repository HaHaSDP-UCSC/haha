/**
 * \file
 *
 * \brief SAM GPIO External Interrupt
 *
 * Copyright (C) 2016 -2017 Atmel Corporation. All rights reserved.
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

#include <utils.h>
#include <utils_assert.h>
#include <hpl_gpio_config.h>
#include <hal_ext_irq.h>

#ifndef CONF_EXTIRQ_0_ENABLE
#define CONF_EXTIRQ_0_ENABLE 0
#endif
#ifndef CONF_EXTIRQ_1_ENABLE
#define CONF_EXTIRQ_1_ENABLE 0
#endif
#ifndef CONF_EXTIRQ_2_ENABLE
#define CONF_EXTIRQ_2_ENABLE 0
#endif

#define EXT_IRQ_AMOUNT (0 + 1)

struct ext_irq_configuration {
	uint8_t  number;
	uint16_t irq_enable;
	/* Write 1 to clear the interrupt type bit */
	uint16_t clear_type_reg;
	/* Write 1 to clear interrupt polarity bit */
	uint16_t clear_pol_reg;
	/* Write 1 to set the interrupt type bit */
	uint16_t set_type_reg;
	/* Write 1 to set interrupt polarity bit */
	uint16_t set_pol_reg;
};

#if EXT_IRQ_AMOUNT < 1
/** Dummy array to pass compiling. */
static const struct ext_irq_configuration _ext_irq[1] = {{0}};
#else
static const struct ext_irq_configuration _ext_irq[] = {
#if CONF_EXTIRQ_0_ENABLE == 1
    {0,
     CONF_EXTIRQ_0_IRQ_ENABLE,
     CONF_EXTIRQ_0_INTTYPECLR,
     CONF_EXTIRQ_0_INTPOLCLR,
     CONF_EXTIRQ_0_INTTYPESET,
     CONF_EXTIRQ_0_INTPOLSET},
#endif
#if CONF_EXTIRQ_1_ENABLE == 1
    {1,
     CONF_EXTIRQ_1_IRQ_ENABLE,
     CONF_EXTIRQ_1_INTTYPECLR,
     CONF_EXTIRQ_1_INTPOLCLR,
     CONF_EXTIRQ_1_INTTYPESET,
     CONF_EXTIRQ_1_INTPOLSET},
#endif
#if CONF_EXTIRQ_2_ENABLE == 1
    {2,
     CONF_EXTIRQ_2_IRQ_ENABLE,
     CONF_EXTIRQ_2_INTTYPECLR,
     CONF_EXTIRQ_2_INTPOLCLR,
     CONF_EXTIRQ_2_INTTYPESET,
     CONF_EXTIRQ_2_INTPOLSET},
#endif
};
#endif

/* Find the first bit set */
static int _ffs(uint16_t *v, uint8_t len)
{
	uint32_t i, j, bit;

	for (i = 0; i < len; i++) {
		bit = 1;
		for (j = 0; j < sizeof(uint16_t) * 8; j++) {
			if (v[i] & bit) {
				return i * 16 + j;
			}
			bit <<= 1;
		}
	}

	return -1;
}

/**
 * \brief Invalid external interrupt and pin numbers
 */
#define INVALID_EXTINT_NUMBER 0xFF
#define INVALID_PIN_NUMBER 0xFFFFFFFF

/**
 * \brief EXTINTx and pin number map
 */
struct _ext_map {
	uint8_t  extint;
	uint32_t pin;
};

/**
 * \brief PIN and EXTINT map for enabled external interrupts
 */
#if EXT_IRQ_AMOUNT < 1
/** Dummy array to pass compiling. */
static const struct _ext_map _map[1] = {{0}};
#else
static const struct _ext_map _map[] = {
#if CONF_EXTIRQ_0_ENABLE == 1
    CONF_EXTIRQ_0_EXTINT_MAP
#endif
#if CONF_EXTIRQ_1_ENABLE == 1
            CONF_EXTIRQ_1_EXTINT_MAP
#endif
#if CONF_EXTIRQ_2_ENABLE == 1
            CONF_EXTIRQ_2_EXTINT_MAP
#endif
};
#endif

/**
 * \brief This function pointer point the AON GPIO wakeup handler function which in the ROM CODE
 */
static void (*aon_handle_ext_wakeup_isr)(void) = (void (*)(void))0x1bc51;

/**
 * \brief The callback to upper layer's interrupt processing routine
 */
static void (*callback)(const uint32_t pin);

/**
 * \internal Get the GPIO hardware instance
 *
 * \param[in] pin The GPIO pin
 *
 * \return The instance of GPIO hardware
 */
static inline void *_get_hardware_instance(const uint8_t pin)
{
	ASSERT(pin < 48);

	if (pin < 16) {
		return GPIO0;
	} else if (pin < 32) {
		return GPIO1;
	} else if (pin < 48) {
		return GPIO2;
	}
	return NULL;
}

/**
 * \internal Retrieve ordinal number of the given gpio hardware instance
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return The index of hardware instance
 */
static uint8_t _gpio_get_hardware_index(const void *const hw)
{
	ASSERT(hw);

	if (hw == GPIO2) {
		return 2;
	}

	return ((uint32_t)hw - (uint32_t)GPIO0) >> 12;
}

/**
 * \internal Retrieve ordinal number of the given gpio hardware instance
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return The ordinal number of the given gpio hardware instance
 */
static int8_t _gpio_get_index(const void *const hw)
{
	ASSERT(hw);

	uint8_t offset = _gpio_get_hardware_index(hw);
	uint8_t i;

	for (i = 0; i < ARRAY_SIZE(_ext_irq); i++) {
		if (_ext_irq[i].number == offset) {
			return i;
		}
	}

	return ERR_INVALID_DATA;
}

/**
 * \brief External interrupt handler
 *
 * \param[in] Interrupt handler parameter
 */
static void _ext_irq_handler(void)
{
	volatile uint16_t flags[3];
	int8_t            pos;
	uint32_t          pin;

	ASSERT(callback);
	flags[0] = hri_gpio_read_INTSTATUSCLEAR_reg(GPIO0);
	hri_gpio_clear_INTSTATUSCLEAR_reg(GPIO0, flags[0]);

	flags[1] = hri_gpio_read_INTSTATUSCLEAR_reg(GPIO1);
	hri_gpio_clear_INTSTATUSCLEAR_reg(GPIO1, flags[1]);

	flags[2] = hri_gpio_read_INTSTATUSCLEAR_reg(GPIO2);
	hri_gpio_clear_INTSTATUSCLEAR_reg(GPIO2, flags[2]);

	/* For AON wakeup pin clear interrupt */
	if (flags[1] & (GPIO_AO_GPIO_0 | GPIO_AO_GPIO_1 | GPIO_AO_GPIO_2)) {
		aon_handle_ext_wakeup_isr();
	}

	while (flags[0] || flags[1] || flags[2]) {
		pos = _ffs((uint16_t *)flags, 3);
		while (-1 != pos) {
			uint8_t lower = 0, middle, upper = EXT_IRQ_AMOUNT;

			pin = INVALID_PIN_NUMBER;
			while (upper >= lower) {
				middle = (upper + lower) >> 1;

				if (_map[middle].extint == pos) {
					pin = _map[middle].pin;
					break;
				}

				if (_map[middle].extint < pos) {
					lower = middle + 1;
				} else {
					upper = middle - 1;
				}
			}

			if (INVALID_PIN_NUMBER != pin) {
				callback(pin);
			}

			flags[pos >> 4] &= ~(1 << (pos % 16));
			pos = _ffs((uint16_t *)flags, 3);
		}
		flags[0] = hri_gpio_read_INTSTATUSCLEAR_reg(GPIO0);
		hri_gpio_clear_INTSTATUSCLEAR_reg(GPIO0, flags[0]);

		flags[1] = hri_gpio_read_INTSTATUSCLEAR_reg(GPIO1);
		hri_gpio_clear_INTSTATUSCLEAR_reg(GPIO1, flags[1]);

		flags[2] = hri_gpio_read_INTSTATUSCLEAR_reg(GPIO2);
		hri_gpio_clear_INTSTATUSCLEAR_reg(GPIO2, flags[2]);
	}
}

/**
* \internal External IRQ interrupt handler
*/
//void GPIO1_Handler(void)
//{
//	_ext_irq_handler();
//}

/**
 * \internal Initialize GPIO instance
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return The status of instance initial
 * \retval ERR_NONE Initial status OK
 * \retval ERR_DENIED No need to initial
 */
static int32_t _gpio_init(void *const hw)
{
	int8_t i;

	ASSERT(hw);

	i = _gpio_get_index(hw);
	if (i < 0) {
		return ERR_DENIED;
	}

	hri_gpio_write_INTTYPECLR_reg(hw, _ext_irq[i].clear_type_reg);
	hri_gpio_write_INTPOLCLR_reg(hw, _ext_irq[i].clear_pol_reg);
	hri_gpio_write_INTTYPESET_reg(hw, _ext_irq[i].set_type_reg);
	hri_gpio_write_INTPOLSET_reg(hw, _ext_irq[i].set_pol_reg);

	if (hw == GPIO1) {
		hri_aongpregsgpregs_write_AON_PINMUX_SEL_reg(
		    AON_GP_REGS0,
		    0 | AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL((_ext_irq[i].irq_enable >> 15) & 1)
		        | AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL((_ext_irq[i].irq_enable >> 14) & 1)
		        | AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL((_ext_irq[i].irq_enable >> 13) & 1));
	} else if (hw == GPIO2) {
		hri_aongpregsgpregs_clear_MS_GPIO_MODE_reg(AON_GP_REGS0, (_ext_irq[i].irq_enable >> 12));
	}

	i = _gpio_get_hardware_index(hw) + GPIO0_IRQn;
	NVIC_DisableIRQ((IRQn_Type)i);
	NVIC_ClearPendingIRQ((IRQn_Type)i);
	NVIC_EnableIRQ((IRQn_Type)i);

	return ERR_NONE;
}

int32_t _ext_irq_init(void (*cb)(const uint32_t pin))
{
	_gpio_init(GPIO0);
	_gpio_init(GPIO1);
	_gpio_init(GPIO2);

	callback = cb;

	return ERR_NONE;
}

int32_t _ext_irq_deinit(void)
{
	NVIC_DisableIRQ(GPIO0_IRQn);
	NVIC_DisableIRQ(GPIO1_IRQn);
	NVIC_DisableIRQ(GPIO2_IRQn);
	callback = NULL;

	return ERR_NONE;
}

int32_t _ext_irq_enable(const uint32_t pin, const bool enable)
{
	uint8_t extint = INVALID_EXTINT_NUMBER;
	uint8_t i      = 0;
	void *  hw     = GPIO0;

	ASSERT(pin < 48);

	for (; i < ARRAY_SIZE(_map); i++) {
		if (_map[i].pin == pin) {
			extint = _map[i].extint;
			break;
		}
	}
	if (INVALID_EXTINT_NUMBER == extint) {
		return ERR_INVALID_DATA;
	}

	hw = _get_hardware_instance(extint);
	extint %= 16;

	if (enable) {
		hri_gpio_write_INTENSET_reg(hw, 1ul << extint);
	} else {
		hri_gpio_write_INTENCLR_reg(hw, 1ul << extint);
		hri_gpio_clear_INTSTATUSCLEAR_reg(hw, 1ul << extint);
	}

	return ERR_NONE;
}
