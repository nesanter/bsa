#ifndef DRV_TIMER_H
#define DRV_TIMER_H

int drv_timer_write(int val, char *str);
int drv_timer_enable_write(int val, char *str);
int drv_timer_select_write(int val, char *str);
int drv_timer_period_write(int val, char *str);

int drv_timer_read();
int drv_timer_enable_read();
int drv_timer_select_read();
int drv_timer_period_read();

int drv_timer_block();

#endif /* DRV_TIMER_H */
