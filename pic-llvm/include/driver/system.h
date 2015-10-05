#ifndef DRV_SYSTEM_H

int drv_sys_delay_write(int val, char *str);
int drv_sys_led_write(int val, char *str);
int drv_sys_tick_write(int val, char *str);
int drv_sys_exit_write(int val, char *str);

int drv_sys_led_read();
int drv_sys_tick_read();

#endif /* DRV_SYSTEM_H */
