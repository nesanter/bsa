#ifndef DRV_PULSE_H
#define DRV_PULSE_H

int drv_pulse_square_enable_write(int val, char *str);
int drv_pulse_pwm_enable_write(int val, char *str);
int drv_pulse_select_write(int val, char *str);
int drv_pulse_map_write(int val, char *str);
int drv_pulse_compare_write(int val, char *str);
int drv_pulse_active_write(int val, char *str);
int drv_pulse_select_read();
int drv_pulse_compare_read();
int drv_pulse_active_read();

#endif /* DRV_PULSE_H */
