#ifndef DRV_GFX_H
#define DRV_GFX_H

int drv_gfx_enable_write(int val, char * str);
int drv_gfx_bb_write(int val, char * str);
int drv_gfx_enqueue_write(int val, char * str);
int drv_gfx_flush_write(int val, char * str);
int drv_gfx_char_write(int val, char * str);
int drv_gfx_enqueue_read();

#endif /* DRV_GFX_H */
