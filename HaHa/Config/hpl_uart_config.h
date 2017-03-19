/* Auto-generated config file hpl_uart_config.h */
#ifndef HPL_UART_CONFIG_H
#define HPL_UART_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#include <peripheral_clk_config.h>
#include <hpl_lpmcu_misc_regs_config.h>

#ifndef CONF_UART_0_ENABLE
#define CONF_UART_0_ENABLE 1
#endif

// <h> Basic Configuration

// <q> Receive buffer enable
// <i> Enable input buffer in module
// <id> uart_rx_enable
#ifndef CONF_UART_0_RXEN
#define CONF_UART_0_RXEN 1
#endif

// <q> Transmitt buffer enable
// <i> Enable output buffer in module
// <id> uart_tx_enable
#ifndef CONF_UART_0_TXEN
#define CONF_UART_0_TXEN 1
#endif

// <o> Frame parity
// <0x0=>Even parity
// <0x1=>Odd parity
// <0x2=>No parity
// <i> Parity bit mode for UART frame
// <id> uart_parity
#ifndef CONF_UART_0_PARITY
#define CONF_UART_0_PARITY 0x0
#endif

// <o> Character Size
// <0x0=>8 bits
// <0x1=>7 bits
// <i> Data character size in UART frame
// <id> uart_character_size
#ifndef CONF_UART_0_CHSIZE
#define CONF_UART_0_CHSIZE 0x0
#endif

// <o> Stop Bit
// <0=>One stop bit
// <1=>Two stop bits
// <i> Number of stop bits in UART frame
// <id> uart_stop_bit
#ifndef CONF_UART_0_SBMODE
#define CONF_UART_0_SBMODE 0
#endif

// <o> Baud rate <1-3000000>
// <i> UART baud rate setting
// <id> uart_baud_rate
#ifndef CONF_UART_0_BAUD_RATE
#define CONF_UART_0_BAUD_RATE 115200
#endif

// </h>

// <e> Advanced configuration
// <id> uart_advanced
#ifndef CONF_UART_0_ADVANCED_CONFIG
#define CONF_UART_0_ADVANCED_CONFIG 0
#endif

// <q> Flow control
// <i> Flow control setting
// <id> uart_arch_flow_control
#ifndef CONF_UART_0_FLOW_CONTROL
#define CONF_UART_0_FLOW_CONTROL 0
#endif
// </e>

#ifndef CONF_UART_0_CLOCK_FREQUENCY
#define CONF_UART_0_CLOCK_FREQUENCY CONF_UART0_FREQUENCY
#endif

#ifndef CONF_UART_0_CLOCK_PRESCALER
#define CONF_UART_0_CLOCK_PRESCALER CONF_UART0_PRESCALER
#endif

#include <peripheral_clk_config.h>
#include <hpl_lpmcu_misc_regs_config.h>

#ifndef CONF_UART_1_ENABLE
#define CONF_UART_1_ENABLE 1
#endif

// <h> Basic Configuration

// <q> Receive buffer enable
// <i> Enable input buffer in module
// <id> uart_rx_enable
#ifndef CONF_UART_1_RXEN
#define CONF_UART_1_RXEN 1
#endif

// <q> Transmitt buffer enable
// <i> Enable output buffer in module
// <id> uart_tx_enable
#ifndef CONF_UART_1_TXEN
#define CONF_UART_1_TXEN 1
#endif

// <o> Frame parity
// <0x0=>Even parity
// <0x1=>Odd parity
// <0x2=>No parity
// <i> Parity bit mode for UART frame
// <id> uart_parity
#ifndef CONF_UART_1_PARITY
#define CONF_UART_1_PARITY 0x1
#endif

// <o> Character Size
// <0x0=>8 bits
// <0x1=>7 bits
// <i> Data character size in UART frame
// <id> uart_character_size
#ifndef CONF_UART_1_CHSIZE
#define CONF_UART_1_CHSIZE 0x0
#endif

// <o> Stop Bit
// <0=>One stop bit
// <1=>Two stop bits
// <i> Number of stop bits in UART frame
// <id> uart_stop_bit
#ifndef CONF_UART_1_SBMODE
#define CONF_UART_1_SBMODE 0
#endif

// <o> Baud rate <1-3000000>
// <i> UART baud rate setting
// <id> uart_baud_rate
#ifndef CONF_UART_1_BAUD_RATE
#define CONF_UART_1_BAUD_RATE 115200
#endif

// </h>

// <e> Advanced configuration
// <id> uart_advanced
#ifndef CONF_UART_1_ADVANCED_CONFIG
#define CONF_UART_1_ADVANCED_CONFIG 1
#endif

// <q> Flow control
// <i> Flow control setting
// <id> uart_arch_flow_control
#ifndef CONF_UART_1_FLOW_CONTROL
#define CONF_UART_1_FLOW_CONTROL 1
#endif
// </e>

#ifndef CONF_UART_1_CLOCK_FREQUENCY
#define CONF_UART_1_CLOCK_FREQUENCY CONF_UART1_FREQUENCY
#endif

#ifndef CONF_UART_1_CLOCK_PRESCALER
#define CONF_UART_1_CLOCK_PRESCALER CONF_UART1_PRESCALER
#endif

// <<< end of configuration section >>>

#endif // HPL_UART_CONFIG_H
