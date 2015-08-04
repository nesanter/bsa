#ifndef DRV_CONSOLE_H
#define DRV_CONSOLE_H

int drv_sys_delay_write(int val, char *str);

int drv_console_write(int val, char *str);
int drv_console_raw_write(int val, char *str);
int drv_console_tx_block_write(int val, char *str);
int drv_console_rx_block_write(int val, char *str);
int drv_console_read();
int drv_console_tx_ready();
int drv_console_tx_block_read();
int drv_console_rx_block_read();
int drv_console_rx_ready();
int drv_console_rx_read();

#endif
