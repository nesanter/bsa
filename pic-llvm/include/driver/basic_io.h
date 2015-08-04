#ifndef BASIC_IO_H
#define BASIC_IO_H

int drv_led_select_write(int val, char *str);
int drv_led_write(int val, char *str);
int drv_led_select_read();
int drv_led_read();

#endif /* BASIC_IO_H */
