/**
 * \file
 *
 * \brief PWM Sync functionality implementation.
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

#include "hal_pwm_sync.h"
#include <utils_assert.h>
#include <utils.h>

/**
 * \brief Driver version
 */
#define DRIVER_VERSION 0x00000001u

/**
 * \brief Initialize pwm
 */
int32_t pwm_sync_init(struct pwm_sync_descriptor *const descr, void *const hw)
{
	ASSERT(descr && hw);

	return _pwm_sync_init(&descr->device, hw);
}

/**
 * \brief Deinitialize pwm
 */
int32_t pwm_sync_deinit(struct pwm_sync_descriptor *const descr)
{
	ASSERT(descr);

	_pwm_sync_deinit(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Start pwm
 */
int32_t pwm_sync_enable(struct pwm_sync_descriptor *const descr)
{
	ASSERT(descr);

	if (_pwm_sync_is_enabled(&descr->device)) {
		return ERR_DENIED;
	}
	_pwm_sync_enable(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Stop pwm
 */
int32_t pwm_sync_disable(struct pwm_sync_descriptor *const descr)
{
	ASSERT(descr);

	if (!_pwm_sync_is_enabled(&descr->device)) {
		return ERR_DENIED;
	}
	_pwm_sync_disable(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Change PWM parameter
 */
int32_t pwm_sync_set_parameters(struct pwm_sync_descriptor *const descr, const uint32_t period,
                                const uint32_t duty_cycle)
{
	ASSERT(descr);

	_pwm_sync_set_param(&descr->device, period, duty_cycle);

	return ERR_NONE;
}

/**
 * \brief Retrieve the current driver version
 */
uint32_t pwm_sync_get_version(void)
{
	return DRIVER_VERSION;
}
