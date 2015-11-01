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

int drv_ldr_read();
int drv_ldr_enable_write();

/*
int drv_piezo_enable_write(int val, char *str);
int drv_piezo_width_write(int val, char *str);
int drv_piezo_active_write(int val, char *str);
int drv_piezo_enable_read();
int drv_piezo_width_read();
int drv_piezo_active_read();
*/

int drv_sw_block();

/*
int drv_pwm_enable_write(int val, char *str);
int drv_pwm_pulse0_write(int val, char *str);
int drv_pwm_pulse1_write(int val, char *str);
*/

#endif /* BASIC_IO_H */
