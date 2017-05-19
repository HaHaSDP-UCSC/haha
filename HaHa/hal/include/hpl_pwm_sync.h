/**
 * \file
 *
 * \brief PWM related functionality declaration.
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
#ifndef _HPL_PWM_SYNC_H_INCLUDED
#define _HPL_PWM_SYNC_H_INCLUDED

/**
 * \addtogroup HPL PWM
 *
 * \section hpl_pwm_sync_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief PWM sync descriptor device structure
 */
struct _pwm_sync_device {
	void *hw;
};

/**
 * \brief Initialize PWM
 *
 * This function does PWM configuration.
 *
 * \param[in] device The pointer to PWM device instance
 * \param[in] hw     The pointer to hardware instance
 *
 * \return Initialization status.
 */
int32_t _pwm_sync_init(struct _pwm_sync_device *const device, void *const hw);

/**
 * \brief Deinitialize TC
 *
 * \param[in] device The pointer to PWM device instance
 */
void _pwm_sync_deinit(struct _pwm_sync_device *const device);

/**
 * \brief Start hardware pwm
 *
 * \param[in] device The pointer to PWM device instance
 */
void _pwm_sync_enable(struct _pwm_sync_device *const device);

/**
 * \brief Stop hardware pwm
 *
 * \param[in] device The pointer to PWM device instance
 */
void _pwm_sync_disable(struct _pwm_sync_device *const device);

/**
 * \brief Set pwm parameter
 *
 * \param[in] device      The pointer to PWM device instance
 * \param[in] period      Programmable PWM update period
 * \param[in] duty_cycle  Duty cycle
 */
void _pwm_sync_set_param(struct _pwm_sync_device *const device, const uint32_t period, const uint32_t duty_cycle);

/**
 * \brief Check if pwm is working
 *
 * \param[in] device The pointer to PWM device instance
 *
 * \return Check status.
 * \retval true The given pwm is working
 * \retval false The given pwm is not working
 */
bool _pwm_sync_is_enabled(const struct _pwm_sync_device *const device);

#ifdef __cplusplus
}
#endif
/**@}*/
#endif /* _HPL_PWM_H_INCLUDED */
