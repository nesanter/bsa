#ifndef GFX_H
#define GFX_H

enum { GFX_WIDTH = 128 };
enum { GFX_HEIGHT = 64 };

typedef unsigned char gfx_cmd;

enum gfx_cmd {
    GFX_CMD_DISPLAY_OFF = 0xAE,
    GFX_CMD_SET_DISPLAY_CLOCKDIV = 0xD5,
    GFX_CMD_SET_MULTIPLEX = 0xA8,
    GFX_CMD_SET_DISPLAY_OFFSET = 0xD3,
    GFX_CMD_SET_START_LINE = 0x40,
    GFX_CMD_SET_CHARGEPUMP = 0x8D,
    GFX_CMD_MEMORY_MODE = 0x20,
    GFX_CMD_SET_COLUMN_ADDRESS = 0x21,
    GFX_CMD_SET_PAGE_ADDRESS = 0x22,
    GFX_CMD_SEGMENT_REMAP = 0xA0,
    GFX_CMD_COM_DIRECTION = 0xC0,
    GFX_CMD_COM_PINS = 0xDA,
    GFX_CMD_SET_CONTRAST = 0x81,
    GFX_CMD_SET_PRECHARGE = 0xD9,
    GFX_CMD_SET_VCOM_DETECT = 0xDB,
    GFX_CMD_SET_DISPLAY_ALLOW_RESUME = 0xA4,
    GFX_CMD_NORMAL_DISPLAY = 0xA6,
    GFX_CMD_DISPLAY_ON = 0xAF
};

void gfx_setup(void);
void gfx_init_sequence(void);
void gfx_bb_set(unsigned char scol, unsigned char ecol, unsigned char spage, unsigned char epage);
void gfx_write(unsigned char * data, unsigned int len);

#endif /* GFX_H */
