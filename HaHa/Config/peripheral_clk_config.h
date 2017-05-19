/* Auto-generated config file peripheral_clk_config.h */
#ifndef PERIPHERAL_CLK_CONFIG_H
#define PERIPHERAL_CLK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

/**
 * \def CONF_CPU_FREQUENCY
 * \brief CPU's Clock frequency
 */
#ifndef CONF_CPU_FREQUENCY
#define CONF_CPU_FREQUENCY 26000000
#endif

// <h> PWM0 Clock Settings
// <y> PWM0 Clock source
// <PWM0_CLOCK_SOURCE"> PWM0 Divider
// <i> This defines the clock source for the PWM0
// <id> pwm_clock_source
#ifndef CONF_PWM0_SRC
#define CONF_PWM0_SRC PWM0_CLOCK_SOURCE
#endif

// </h>

/**
 * \def PWM0 FREQUENCY
 * \brief PWM0's Clock frequency
 */
#ifndef CONF_PWM0_FREQUENCY
#define CONF_PWM0_FREQUENCY 26000000
#endif

// <h> UART0 Clock Settings
// <y> UART0 Clock source
// <UART0_CLOCK_SOURCE">UART0 Divider
// <i> This defines the clock source for the UART0
// <id> uart_clock_source
#ifndef CONF_UART0_SRC
#define CONF_UART0_SRC UART0_CLOCK_SOURCE
#endif

// </h>

/**
 * \def UART0 FREQUENCY
 * \brief UART0's Clock frequency
 */
#ifndef CONF_UART0_FREQUENCY
#define CONF_UART0_FREQUENCY 26000000
#endif

// <h> UART1 Clock Settings
// <y> UART1 Clock source
// <UART1_CLOCK_SOURCE">UART1 Divider
// <i> This defines the clock source for the UART1
// <id> uart_clock_source
#ifndef CONF_UART1_SRC
#define CONF_UART1_SRC UART1_CLOCK_SOURCE
#endif

// </h>

/**
 * \def UART1 FREQUENCY
 * \brief UART1's Clock frequency
 */
#ifndef CONF_UART1_FREQUENCY
#define CONF_UART1_FREQUENCY 26000000
#endif

// <<< end of configuration section >>>

#endif // PERIPHERAL_CLK_CONFIG_H
