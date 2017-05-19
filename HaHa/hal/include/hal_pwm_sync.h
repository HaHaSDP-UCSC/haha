/**
 * \file
 *
 * \brief PWM Sync functionality declaration.
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

#ifndef HAL_PWM_SYNC_H_INCLUDED
#define HAL_PWM_SYNC_H_INCLUDED

#include <hpl_pwm_sync.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_pwm_sync
 *
 *@{
 */

/**
 * \brief PWM descriptor
 *
 * The PWM descriptor forward declaration.
 */
struct pwm_sync_descriptor;

/**
 * \brief PWM descriptor
 */
struct pwm_sync_descriptor {
	struct _pwm_sync_device device;
};

/**
 *  \brief Initialize the PWM HAL instance and hardware
 *
 *  \param[in] descr The PWM device
 *  \param[in] hw    The pointer to hardware instance
 *
 *  \return Operation status.
 */
int32_t pwm_sync_init(struct pwm_sync_descriptor *const descr, void *const hw);

/**
 *  \brief Deinitialize the PWM HAL instance and hardware
 *
 *  \param[in] descr The PWM device
 *
 *  \return Operation status.
 */
int32_t pwm_sync_deinit(struct pwm_sync_descriptor *const descr);

/**
 *  \brief PWM output start
 *
 *  \param[in] descr The PWM device
 *
 *  \return Operation status.
 */
int32_t pwm_sync_enable(struct pwm_sync_descriptor *const descr);

/**
 *  \brief PWM output stop
 *
 *  \param[in] descr The PWM device
 *
 *  \return Operation status.
 */
int32_t pwm_sync_disable(struct pwm_sync_descriptor *const descr);

/**
 *  \brief Change PWM parameter
 *
 *  \param[in] descr       The PWM device
 *  \param[in] period      Programmable PWM update period
 *  \param[in] duty_cycle  Duty cycle
 *
 *  \return Operation status.
 */
int32_t pwm_sync_set_parameters(struct pwm_sync_descriptor *const descr, const uint32_t period,
                                const uint32_t duty_cycle);

/**
 *  \brief Get PWM driver version
 *
 *  \return Current driver version.
 */
uint32_t pwm_sync_get_version(void);
/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* HAL_PWM_SYNC_H_INCLUDED */
