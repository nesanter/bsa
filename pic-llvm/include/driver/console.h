#ifndef DRV_CONSOLE_H
#define DRV_CONSOLE_H

int drv_console_write(int val, char *str);
int drv_console_raw_write(int val, char *str);
int drv_console_read();
int drv_console_tx_ready();

#endif
