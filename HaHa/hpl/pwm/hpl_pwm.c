/**
 * \file
 *
 * \brief SAM PWM
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

#include <utils_assert.h>
#include <utils.h>
#include <hpl_pwm_config.h>
#include <hpl_pwm_sync.h>

#define NUM_PWM 4

/**
 * \brief Macro is used to fill usart configuration structure based on its
 * number
 *
 * \param[in] n The number of structures
 */
#define PWM_SYNC_CONFIGURATION(n)                                                                                      \
	{                                                                                                                  \
		(n), CONF_PWM_##n##_OUTPUT_POLARITY, CONF_PWM_##n##_AGCDATA_FMT, CONF_PWM_##n##_SAMPLE_METHOD,                 \
		    CONF_PWM_##n##_PERIOD, CONF_PWM_##n##_DUTY_CYCLE, CONF_PWM_##n##_PRESCALER,                                \
	}

/**
 * \brief PWM configuration type
 */
struct pwm_sync_configuration {
	uint8_t number;
	/** Inverse the polarity */
	uint32_t output_polarity;
	/** AGC data format */
	uint32_t agcdata_format;
	/** Sample method */
	uint32_t sample_method;
	/** Programmable PWM update period */
	uint32_t period;
	/** This value specifies the duty cycle(%) */
	uint32_t duty_cycle;
	/** PWM source clock frequency select */
	uint32_t prescaler;
};

/**
 * \brief Array of PWM configurations
 */
static struct pwm_sync_configuration _pwm_sync[] = {
#if CONF_PWM_0_ENABLE == 1
    PWM_SYNC_CONFIGURATION(0),
#endif
#if CONF_PWM_1_ENABLE == 1
    PWM_SYNC_CONFIGURATION(1),
#endif
#if CONF_PWM_2_ENABLE == 1
    PWM_SYNC_CONFIGURATION(2),
#endif
#if CONF_PWM_3_ENABLE == 1
    PWM_SYNC_CONFIGURATION(3),
#endif
};

/**
 * \internal Retrieve PWM index
 *
 * \param[in] device The pointer to hardware instance
 *
 * \return The index of PWM configuration
 */
static int8_t _get_pwm_sync_index(void *const device)
{
	for (uint8_t i = 0; i < NUM_PWM; i++) {
		if (_pwm_sync[i].number == (uint32_t)device) {
			return i;
		}
	}

	ASSERT(false);
	return -1;
}

/**
 * \internal Calculate agcdata_in
 *
 * \param[in] _pwm_cfg   The pointer to array of PWM configurations
 *
 * \return The value of agcdata_in
 */
static inline int32_t _get_agcdata_in(struct pwm_sync_configuration *const _pwm_cfg)
{
	uint32_t data = 1024 * _pwm_cfg->duty_cycle;

	/* data = data/100 */
	data = (((((uint32_t)(data) * (uint32_t)0x47AF) >> 16U) + (data)) >> 1) >> 6;
	if (_pwm_cfg->agcdata_format) {
		return (uint16_t)(data);
	} else {
		return (int16_t)(data - 512);
	}
}

/**
 * \brief Initialize synchronous PWM
 */
int32_t _pwm_sync_init(struct _pwm_sync_device *const device, void *const hw)
{
	int32_t agcdata_in;
	int8_t  i = _get_pwm_sync_index(hw);

	device->hw = hw;
	ASSERT(ARRAY_SIZE(_pwm_sync));

	/*
	 * The reason of using the type of struct:
	 * - agcdata_in is int16_t
	 * - The value of agcdata_in need to be calculated
	 * - In _pwm_sync_set_param function, need use duty_cycle and agcdata_format parameters
	 */
	agcdata_in = _get_agcdata_in(&_pwm_sync[i]);
	switch ((uint32_t)device->hw) {
	case 0:
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_OUTPUT_POLARITY_bit(LPMCU_MISC_REGS0, _pwm_sync[i].output_polarity);
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_AGCDATA_FMT_bit(LPMCU_MISC_REGS0, _pwm_sync[i].agcdata_format);
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_SAMPLE_METHOD_bit(LPMCU_MISC_REGS0, _pwm_sync[i].sample_method);
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_CLOCK_SEL_bf(LPMCU_MISC_REGS0, _pwm_sync[i].prescaler);
		break;

	case 1:
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_OUTPUT_POLARITY_bit(LPMCU_MISC_REGS0, _pwm_sync[i].output_polarity);
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_AGCDATA_FMT_bit(LPMCU_MISC_REGS0, _pwm_sync[i].agcdata_format);
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_SAMPLE_METHOD_bit(LPMCU_MISC_REGS0, _pwm_sync[i].sample_method);
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_CLOCK_SEL_bf(LPMCU_MISC_REGS0, _pwm_sync[i].prescaler);
		break;

	case 2:
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_OUTPUT_POLARITY_bit(LPMCU_MISC_REGS0, _pwm_sync[i].output_polarity);
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_AGCDATA_FMT_bit(LPMCU_MISC_REGS0, _pwm_sync[i].agcdata_format);
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_SAMPLE_METHOD_bit(LPMCU_MISC_REGS0, _pwm_sync[i].sample_method);
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_CLOCK_SEL_bf(LPMCU_MISC_REGS0, _pwm_sync[i].prescaler);
		break;

	case 3:
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_OUTPUT_POLARITY_bit(LPMCU_MISC_REGS0, _pwm_sync[i].output_polarity);
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_AGCDATA_FMT_bit(LPMCU_MISC_REGS0, _pwm_sync[i].agcdata_format);
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_SAMPLE_METHOD_bit(LPMCU_MISC_REGS0, _pwm_sync[i].sample_method);
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_CLOCK_SEL_bf(LPMCU_MISC_REGS0, _pwm_sync[i].prescaler);
		break;

	default:
		break;
	}

	return ERR_NONE;
}

/**
 * \brief De-initialize PWM
 */
void _pwm_sync_deinit(struct _pwm_sync_device *const device)
{
	ASSERT(device);

	switch ((uint32_t)device->hw) {
	case 0:
		hri_lpmcumiscregsmiscregs_clear_PWM0_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM0_CTRL_reg(LPMCU_MISC_REGS0, LPMCU_MISC_REGS_PWM0_CTRL_MASK);
		break;

	case 1:
		hri_lpmcumiscregsmiscregs_clear_PWM1_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM1_CTRL_reg(LPMCU_MISC_REGS0, LPMCU_MISC_REGS_PWM1_CTRL_MASK);
		break;

	case 2:
		hri_lpmcumiscregsmiscregs_clear_PWM2_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM2_CTRL_reg(LPMCU_MISC_REGS0, LPMCU_MISC_REGS_PWM2_CTRL_MASK);
		break;

	case 3:
		hri_lpmcumiscregsmiscregs_clear_PWM3_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM3_CTRL_reg(LPMCU_MISC_REGS0, LPMCU_MISC_REGS_PWM3_CTRL_MASK);
		break;

	default:
		break;
	}
}

/**
 * \brief Enable PWM
 *
 */
void _pwm_sync_enable(struct _pwm_sync_device *const device)
{
	ASSERT(device);

	switch ((uint32_t)device->hw) {
	case 0:
		hri_lpmcumiscregsmiscregs_set_LPMCU_CLOCK_ENABLES_1_PWM0_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_PWM0_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 1:
		hri_lpmcumiscregsmiscregs_set_LPMCU_CLOCK_ENABLES_1_PWM1_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_PWM1_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 2:
		hri_lpmcumiscregsmiscregs_set_LPMCU_CLOCK_ENABLES_1_PWM2_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_PWM2_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 3:
		hri_lpmcumiscregsmiscregs_set_LPMCU_CLOCK_ENABLES_1_PWM3_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_set_PWM3_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	default:
		break;
	}
}

/**
 * \brief Disable PWM
 *
 */
void _pwm_sync_disable(struct _pwm_sync_device *const device)
{
	switch ((uint32_t)device->hw) {
	case 0:
		hri_lpmcumiscregsmiscregs_clear_LPMCU_CLOCK_ENABLES_1_PWM0_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM0_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 1:
		hri_lpmcumiscregsmiscregs_clear_LPMCU_CLOCK_ENABLES_1_PWM1_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM1_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 2:
		hri_lpmcumiscregsmiscregs_clear_LPMCU_CLOCK_ENABLES_1_PWM2_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM2_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 3:
		hri_lpmcumiscregsmiscregs_clear_LPMCU_CLOCK_ENABLES_1_PWM3_CLK_EN_bit(LPMCU_MISC_REGS0);
		hri_lpmcumiscregsmiscregs_clear_PWM3_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	default:
		break;
	}
}

/**
 * \brief Set PWM parameter
 */
void _pwm_sync_set_param(struct _pwm_sync_device *const device, const uint32_t period, const uint32_t duty_cycle)
{
	/*
	 * Note:
	 * period      Programmable PWM update period (0 ~ 8)
	 * duty_cycle  Duty cycle (%)
	 */
	int8_t  i = _get_pwm_sync_index(device->hw);
	int32_t agcdata_in;

	ASSERT(ARRAY_SIZE(_pwm_sync));

	_pwm_sync[i].period     = period;
	_pwm_sync[i].duty_cycle = duty_cycle;
	agcdata_in              = _get_agcdata_in(&_pwm_sync[i]);

	switch ((uint32_t)device->hw) {
	case 0:
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM0_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		break;

	case 1:
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM1_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		break;

	case 2:
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM2_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		break;

	case 3:
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_PWM_PERIOD_bf(LPMCU_MISC_REGS0, _pwm_sync[i].period);
		hri_lpmcumiscregsmiscregs_write_PWM3_CTRL_AGCDATA_IN_bf(LPMCU_MISC_REGS0, agcdata_in);
		break;

	default:
		break;
	}
}

/**
 * \brief Check if PWM is running
 */
bool _pwm_sync_is_enabled(const struct _pwm_sync_device *const device)
{
	ASSERT(device);

	switch ((uint32_t)device->hw) {
	case 0:
		return hri_lpmcumiscregsmiscregs_get_PWM0_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 1:
		return hri_lpmcumiscregsmiscregs_get_PWM1_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 2:
		return hri_lpmcumiscregsmiscregs_get_PWM2_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	case 3:
		return hri_lpmcumiscregsmiscregs_get_PWM3_CTRL_PWM_EN_bit(LPMCU_MISC_REGS0);
		break;

	default:
		break;
	}

	return false;
}
