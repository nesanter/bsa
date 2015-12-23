#ifndef DRV_ANA_H
#define DRV_ANA_H

int drv_ana_write(int val, char *str);
int drv_ana_enable_write(int val, char *str);
int drv_ana_activate_write(int val, char *str);
int drv_ana_deactivate_write(int val, char *str);

int drv_ana_enable_read();

#endif /* DRV_ANA_H */
