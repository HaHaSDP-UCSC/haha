/**
 * \file
 *
 * \brief SAM B11 Dual Timer
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
 * DAMAGES (INCLUDING, BUT NOT LIMIT ED TO, PROCUREMENT OF SUBSTITUTE GOODS
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
#include <hpl_timer.h>
#include <hpl_dualtimer_config.h>
#include <utils_assert.h>
#include <hpl_irq.h>
#include <stdio.h>
#ifndef CONF_DUALTIMER_0_ENABLE
#define CONF_DUALTIMER_0_ENABLE 0
#endif

#ifndef CONF_DUALTIMER_1_ENABLE
#define CONF_DUALTIMER_1_ENABLE 0
#endif

/** Build configuration from header macros. */
#define DUALTIMER_CONFIGURATION(n)                                                                                     \
	{                                                                                                                  \
		(n), (CONF_DUALTIMER_##n##_COUNTER_SIZE << DUALTIMER_TIMER1CONTROL_TIMER_SIZE_Pos)                             \
		         | (CONF_DUALTIMER_##n##_PRESCALER << DUALTIMER_TIMER1CONTROL_TIMERPRE_Pos),                           \
		    (uint32_t)(CONF_DUALTIMER_##n##_LOAD_VALUE)                                                                \
	}

/**
 * \brief DUALTIMER configuration type
 */
struct dualtimer_configuration {
	uint8_t  number;
	uint8_t  ctrl;
	uint32_t load_value;
};

static const struct dualtimer_configuration _dualtimer[] = {
#if CONF_DUALTIMER_0_ENABLE == 1
    DUALTIMER_CONFIGURATION(0),
#endif
#if CONF_DUALTIMER_1_ENABLE == 1
    DUALTIMER_CONFIGURATION(1),
#endif
};

static struct _timer_device *_dualtimer_device[2];

/**
 * \internal Retrieve ordinal number of the given DUALTIMER hardware instance
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return The number of the DUALTIMER hardware instance
 */
static uint8_t _get_hardware_index(const void *const hw)
{
	ASSERT(hw);

	return ((uint32_t)hw - (uint32_t)DUALTIMER0) >> 5;
}

/**
 * \internal Retrieve ordinal channel index of the DUALTIMER instance
 *
 * \param[in] hw The index of DUALTIMER module
 *
 * \return The ordinal channel index of the DUALTIMER instance
 */
static uint8_t _get_timer_index(void *const hw)
{
	uint8_t offset = _get_hardware_index(hw);
	uint8_t i;

	for (i = 0; i < ARRAY_SIZE(_dualtimer); i++) {
		if (_dualtimer[i].number == offset) {
			return i;
		}
	}

	return 0;
}

/**
 * \internal DUALTIMER interrupt handler
 *
 * \param[in] p Interrupt handler parameter
 */
static void dualtimer_interrupt_handler(void *p)
{
	struct _timer_device *device;
	//static uint16_t count= 0;
	(void)p;

	if (hri_dualtimer_get_TIMER1MIS_TIMER1MIS_bit(DUALTIMER0)) {
		hri_dualtimer_write_TIMER1INTCLR_reg(DUALTIMER0, DUALTIMER_TIMER1INTCLR_MASK);
		device = _dualtimer_device[0];
		if (device->timer_cb.period_expired) {
			//printf("connect expired count: %d\r\n",count++);
			device->timer_cb.period_expired(device);
		}
	}
	if (hri_dualtimer_get_TIMER2MIS_TIMER2MIS_bit(DUALTIMER0)) {
		hri_dualtimer_write_TIMER2INTCLR_reg(DUALTIMER0, DUALTIMER_TIMER2INTCLR_MASK);
		device = _dualtimer_device[1];
		if (device->timer_cb.period_expired) {
			device->timer_cb.period_expired(device);
		}
	}
}

/**
 * \internal DUALTIMER reset
 *
 * This function will reset all the value of DUALTIMER registers
 */
static void _dualtimer_reset(void)
{
	/* Reset the DUALTIMER register */
	hri_lpmcumiscregsmiscregs_clear_LPMCU_GLOBAL_RESET_1_DUALTIMER0_RSTN_bit(LPMCU_MISC_REGS0);
	hri_lpmcumiscregsmiscregs_set_LPMCU_GLOBAL_RESET_1_DUALTIMER0_RSTN_bit(LPMCU_MISC_REGS0);
}

int32_t _timer_init(struct _timer_device *const device, void *const hw)
{
	uint8_t i = _get_timer_index(hw);

	ASSERT(device && hw);

	if (hri_dualtimer_get_TIMER1CONTROL_TIMER_ENABLE_bit(hw)) {
		return ERR_DENIED;
	}

	hri_dualtimer_write_TIMER1CONTROL_reg(hw, _dualtimer[i].ctrl);
	hri_dualtimer_write_TIMER1LOAD_reg(hw, _dualtimer[i].load_value);
	hri_dualtimer_set_TIMER1CONTROL_TIMER_MODE_bit(hw);
	hri_dualtimer_set_TIMER1CONTROL_INTERRUPT_ENABLE_bit(hw);

	i = _get_hardware_index(hw);
	LPMCU_MISC_REGS0->DUALTIMER0_CTRL.reg |= (1 << i);
	_dualtimer_device[i] = device;

	device->hw            = hw;
	device->irq.handler   = dualtimer_interrupt_handler;
	device->irq.parameter = (void *)device;
	_irq_register(DUALTIMER0_IRQn, &device->irq);
	_irq_enable(DUALTIMER0_IRQn);

	return ERR_NONE;
}

void _timer_deinit(struct _timer_device *const device)
{
	ASSERT(device && device->hw);

	hri_dualtimer_clear_TIMER1CONTROL_TIMER_ENABLE_bit(device->hw);
	device->hw = NULL;
	_dualtimer_reset();
}

void _timer_start(struct _timer_device *const device)
{
	ASSERT(device && device->hw);

	hri_dualtimer_set_TIMER1CONTROL_TIMER_ENABLE_bit(device->hw);
}

void _timer_stop(struct _timer_device *const device)
{
	ASSERT(device && device->hw);

	hri_dualtimer_clear_TIMER1CONTROL_TIMER_ENABLE_bit(device->hw);
}

void _timer_set_period(struct _timer_device *const device, const uint32_t clock_cycles)
{
	ASSERT(device && device->hw);

	hri_dualtimer_write_TIMER1LOAD_reg(device->hw, clock_cycles);
}

uint32_t _timer_get_period(const struct _timer_device *const device)
{
	ASSERT(device && device->hw);

	return hri_dualtimer_read_TIMER1LOAD_reg(device->hw);
}

bool _timer_is_started(const struct _timer_device *const device)
{
	ASSERT(device && device->hw);

	return hri_dualtimer_get_TIMER1CONTROL_TIMER_ENABLE_bit(device->hw);
}

void _timer_set_irq(struct _timer_device *const device)
{
	ASSERT(device);

	(void)device;

	_irq_set((IRQn_Type)DUALTIMER0_IRQn);
}

/**
 * \brief Retrieve timer helper functions
 */
struct _timer_hpl_interface *_dt_get_timer(void)
{
	return NULL;
}
