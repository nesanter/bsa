#ifndef UART_H
#define UART_H

#define UART_TX_SUCCESS 0
#define UART_TX_FAILURE 1

#define UART_RX_SUCCESS 0
#define UART_RX_FAILURE 1

void uart_setup(void);

int uart_tx(char c);
void uart_print(char *s);
int uart_rx(char *dest);

int uart_get_enabled(void);

#endif /* UART_H */