#ifndef DRV_IO_H
#define DRV_IO_H

int drv_io_a_write(int val, char *str);
int drv_io_a_set_write(int val, char *str);
int drv_io_a_clear_write(int val, char *str);
int drv_io_a_toggle_write(int val, char *str);
int drv_io_a_mode_write(int val, char *str);
int drv_io_a_mode_in_write(int val, char *str);
int drv_io_a_mode_out_write(int val, char *str);
int drv_io_b_write(int val, char *str);
int drv_io_b_set_write(int val, char *str);
int drv_io_b_clear_write(int val, char *str);
int drv_io_b_toggle_write(int val, char *str);
int drv_io_b_mode_write(int val, char *str);
int drv_io_b_mode_in_write(int val, char *str);
int drv_io_b_mode_out_write(int val, char *str);

#endif
