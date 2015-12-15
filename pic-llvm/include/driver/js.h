#ifndef DRV_JS_H
#define DRV_JS_H

int drv_js_enable_write(int val, char *str);
int drv_js_enable_read(void);
int drv_js_x_read(void);
int drv_js_y_read(void);
int drv_js_sw_read(void);

#endif /* DRV_JS_H */
