#ifndef BASIC_IO_H
#define BASIC_IO_H

int drv_led_select_write(int val, char *str);
int drv_led_write(int val, char *str);
int drv_led_select_read();
int drv_led_read();

int drv_sw_read();
int drv_sw_select_write(int val, char *str);
int drv_sw_edge_write(int val, char *str);
int drv_sw_select_read();
int drv_sw_edge_read();

int drv_sw_block();

#endif /* BASIC_IO_H */
