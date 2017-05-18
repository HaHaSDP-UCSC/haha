/**
 * \file
 *
 * \brief Application BSP for BLE applications for SAMD21
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef APP_BSP_H
#define APP_BSP_H

/* Application Timer ID Definition */
typedef enum { BLE_APP_TIMER_ID1, BLE_APP_TIMER_ID2 } ble_app_timer_t;

typedef enum { BLE_TIMER_ONESHOT, BLE_TIMER_REPEAT } ble_timer_mode_t;

/* Console buffer size */
#define CONSOLE_BUFFER_SIZE (200)

/* BLE Console redirection */

/* No of Software timer required for this application */
#define MAX_SW_TIMER (2)

typedef void (*ble_button_cb_t)(void);

typedef void (*ble_timer_cb_t)(void);

void bsp_init(void);
void serial_console_init(void);
void ble_timer_start(ble_app_timer_t timer_id, uint32_t time, ble_timer_mode_t timer_mode, ble_timer_cb_t timer_cb);

void ble_timer_stop(ble_app_timer_t timer_id);

#endif // APP_BSP_H
