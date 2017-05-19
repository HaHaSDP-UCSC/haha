/* Auto-generated config file hpl_pwm_config.h */
#ifndef HPL_PWM_CONFIG_H
#define HPL_PWM_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#include <peripheral_clk_config.h>
#include <hpl_lpmcu_misc_regs_config.h>

#ifndef CONF_PWM_0_ENABLE
#define CONF_PWM_0_ENABLE 1
#endif

// <h> Basic settings

// <o> Output waveform frequency Setting
// <0x0=> Base frequency is divided by 64
// <0x1=> Base frequency is divided by 128
// <0x2=> Base frequency is divided by 256
// <0x3=> Base frequency is divided by 512
// <0x4=> Base frequency is divided by 1024
// <0x5=> Base frequency is divided by 2048
// <0x6=> Base frequency is divided by 4096
// <0x7=> Base frequency is divided by 8192
// <0x8=> Base frequency is divided by 16384
// <i> This defines the exponent to calculate pwm frequency(fpwm = fbase/(64*2^i)).
// <id> pwm_period
#ifndef CONF_PWM_0_PERIOD
#define CONF_PWM_0_PERIOD 4
#endif

// <o> Waveform Duty Cycle Value (1%) <0-100>
// <i> The unit of this value is 1/100.
// <id> pwm_duty_cycle
#ifndef CONF_PWM_0_DUTY_CYCLE
#define CONF_PWM_0_DUTY_CYCLE 50
#endif
// </h>

// <e> Advanced Settings
// <id> pwm_master_advanced
#ifndef CONF_PWM_0_ADVANCED
#define CONF_PWM_0_ADVANCED 0
#endif

// <q> Inverse Polarity
// <i> Indicates whether inverse output polarity is enabled or not
// <id> pwm_output_polarity
#ifndef CONF_PWM_0_OUTPUT_POLARITY
#define CONF_PWM_0_OUTPUT_POLARITY 0
#endif

// <q> AGC Data Format
// <i> Indicates whether AGC data format is enabled or not
// <id> pwm_agcdata_fmt
#ifndef CONF_PWM_0_AGCDATA_FMT
#define CONF_PWM_0_AGCDATA_FMT 0
#endif

// <o> AGC Data Format
// <0x0=> Samples agcdata at >= 1024 cycles and does not lose precision
// <0x1=> Samples at PWM period but will lose LSBs if less than 1024
// <i> This defines sample method
// <id> pwm_sample_method
#ifndef CONF_PWM_0_SAMPLE_METHOD
#define CONF_PWM_0_SAMPLE_METHOD 0
#endif
// </e>

#ifndef CONF_PWM_0_PRESCALER
#define CONF_PWM_0_PRESCALER CONF_PWM0_PRESCALER
#endif

// <<< end of configuration section >>>

#endif // HPL_PWM_CONFIG_H
