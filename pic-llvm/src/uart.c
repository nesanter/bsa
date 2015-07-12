#include "ulib/uart.h"
#include "ulib/pins.h"
#include "ulib/ulib.h"
#include "config.h"

int uart_enabled = 0;

void uart_setup(void) {
    Pin p_rx = {PIN_GROUP_B, BITS(2)};
    Pin p_tx = {PIN_GROUP_B, BITS(3)};

    pin_mode_set(p_rx, PIN_INPUT, PIN_DIGITAL);
    pin_mode_set(p_tx, PIN_OUTPUT, PIN_DIGITAL);

    u_pps_in_u1rx(PPS_IN3_RPB2);
    u_pps_out_u1tx(PPS_OUT1_RPB3);

    u_uartx_baud_set(UART1, SYSTEM_FREQ, 9600);

    u_uart_config config = u_uartx_load_config(UART1);

    config.on = 1;
    config.parity_data_mode = 0;
    config.stop_mode = 0;

    config.rx_enable = 1;
    config.tx_enable = 1;

    config.rx_int_mode = 0;

    u_uartx_save_config(UART1, config);

    uart_enabled = 1;
}

void uart_setup_rx_interrupts(void) {
    u_uart_int_config config = u_uartx_int_load_config(UART1);

    config.rx_enable = 1;
    config.priority = 2;
    config.subpriority = 0;
    config.rx_clear = 1;

    u_uartx_int_save_config(UART1, config);
}

int uart_tx(char c) {
    if (u_uartx_get_tx_full(UART1))
        return UART_TX_FAILURE;

    u_uartx_tx_register_write(UART1, c);
    
    return UART_TX_SUCCESS;
}

void uart_print(char *s) {
    while (*s) {
        while (!u_uartx_get_tx_full(UART1)) {
            u_uartx_tx_register_write(UART1, *s++);
        }
    }
}

int uart_rx(char *dest) {
    if (u_uartx_get_rx_available(UART1)) {
        *dest = u_uartx_rx_register_read(UART1);
        return UART_RX_SUCCESS;
    }

    return UART_RX_FAILURE;
}

int uart_get_enabled(void) {
    return uart_enabled;
}
