#ifndef DRV_TASK_H

int drv_task_size_write(int val, char *str);
int drv_task_stack_small_write(int val, char *str);
int drv_task_stack_large_write(int val, char *str);
int drv_task_this_depth_write(int val, char *str);

int drv_task_size_read();
int drv_task_stack_small_read();
int drv_task_stack_large_read();
int drv_task_stack_free_read();
int drv_task_count_read();
int drv_task_max_read();
int drv_task_this_depth_read();
int drv_task_this_stack_read();
int drv_task_this_allocation_read();

#endif /* DRV_TASK_H */
