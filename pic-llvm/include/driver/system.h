#ifndef DRV_SYSTEM_H

int drv_sys_delay_write(int val, char *str);
int drv_sys_led_write(int val, char *str);
int drv_sys_tick_write(int val, char *str);
int drv_sys_exit_write(int val, char *str);
int drv_sys_seed_write(int val, char *str);

int drv_sys_led_read();
int drv_sys_tick_read();
int drv_sys_rand8_read();
int drv_sys_rand32_read();

int drv_expm_select_write(int val, char *str);
int drv_expm_emit_write(int val, char *str);
int drv_expm_select_read();
int drv_expm_block();

#endif /* DRV_SYSTEM_H */
