/* Auto-generated config file hpl_dualtimer_config.h */
#ifndef HPL_DUALTIMER_CONFIG_H
#define HPL_DUALTIMER_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#include <peripheral_clk_config.h>
#include <hpl_lpmcu_misc_regs_config.h>

// Enable configuration of module
#ifndef CONF_DUALTIMER_0_ENABLE
#define CONF_DUALTIMER_0_ENABLE 1
#endif

// <h> Basic Configuration

// <o> Length of one timer tick in uS <0-4294967295>
// <id> timer_tick
#ifndef CONF_DUALTIMER_0_TIMER_TICK
#define CONF_DUALTIMER_0_TIMER_TICK 1000
#endif

// <o> Clock Prescaler
// <i> These bits prescale the clock of timer.
// <0x0=> No Clock Divided
// <0x1=> Clock Divided by 16
// <0x2=> Clock Divided by 256
// <id> timer_prescaler
#ifndef CONF_DUALTIMER_0_PRESCALER
#define CONF_DUALTIMER_0_PRESCALER 0
#endif

// </h>

// <e> Advanced configuration
// <id> dualtimer_advanced_configuration
#ifndef CONF_DUALTIMER_0_ADVANCED_CONFIGURATION_ENABLE
#define CONF_DUALTIMER_0_ADVANCED_CONFIGURATION_ENABLE 0
#endif

// <o> Counter Size
// <0x0=> Counter in 16-bit mode
// <0x1=> Counter in 32-bit mode
// <i> This defines the counter size
// <id> dualtimer_counter_size
#ifndef CONF_DUALTIMER_0_COUNTER_SIZE
#define CONF_DUALTIMER_0_COUNTER_SIZE 1
#endif

// </e>

// Calculate timer load value register value from requested timer tick value
#ifndef CONF_DUALTIMER_0_LOAD_VALUE
#define CONF_DUALTIMER_0_LOAD_VALUE                                                                                    \
	(uint32_t)((float)CONF_DUALTIMER0_FREQUENCY                                                                        \
	           / (float)(CONF_DUALTIMER_0_TIMER_TICK * (1 << (4 * CONF_DUALTIMER_0_PRESCALER))))
#endif

#ifndef CONF_DUALTIMER_0_PRESCALER
#define CONF_DUALTIMER_0_PRESCALER CONF_DUALTIMER_PRESCALER
#endif

// <<< end of configuration section >>>

#endif // HPL_DUALTIMER_CONFIG_H
