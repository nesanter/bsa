#include "proc/p32mx250f128b.h"

#include "ulib/gfx.h"
#include "ulib/ulib.h"
#include "ulib/pins.h"
//#include "ulib/spi.h"
#include "ulib/uart.h"
#include "ulib/util.h"

/*  CS  = pin 26 [B15] (auto)
 *  SCK = pin 25 [B14] (auto)
 *  SDO = pin 24 [B13] (auto)
 *  D/C = pin 23 [B12]
 *  SDI = pin 22 [B11] (auto)
 */

const Pin dc = {PIN_GROUP_B, BITS(12)};
//const Pin cs = {PIN_GROUP_B, BITS(1)};
const Pin rst = {PIN_GROUP_B, BITS(0)};

void gfx_setup(void) {
    u_spi_config config = u_spi_load_config(SPI1);

    config.frame_enable = 0;
    config.ss_polarity = 0; // active low
    config.master_ss_enable = 1;
    config.clock_select = 0;
    config.enhanced_buffer = 1;
    config.on = 1;
    config.disable_sdo = 0;
    config.disable_sdi = 1;
    config.mode = 0; //8 bits
    config.sample_phase = 0;
    config.cke = 1;
    config.ckp = 0;
    config.master_enable = 1;
    config.tx_int_mode = 1;

    u_spi_save_config(SPI1, config);

    u_spi_set_baud(SPI1, 1); // 10MHz

//    u_pps_in_sdi1(PPS_IN2_RPB11);
    u_pps_out_sdo1_b(PPS_OUT3_RPB13);
    u_pps_out_ss1(PPS_OUT1_RPB4);

    pin_mode_set(dc, PIN_OUTPUT, PIN_DIGITAL);
//    pin_mode_set(cs, PIN_OUTPUT, PIN_DIGITAL);
    pin_mode_set(rst, PIN_OUTPUT, PIN_DIGITAL);

/*
    u_spi_int_config iconfig = u_spi_int_load_config(SPI1);

    //iconfig.tx_enable = 1;
    iconfig.priority = 3;
    iconfig.subpriority = 0;
    iconfig.clear = 1;

    u_spi_int_save_config(SPI1, iconfig);
 */
}

void gfx_init_sequence(void) {
    static const unsigned char cmd [] = {
        GFX_CMD_DISPLAY_OFF,
        GFX_CMD_SET_DISPLAY_CLOCKDIV, 0x80,
        GFX_CMD_SET_MULTIPLEX, 0x3F,
        GFX_CMD_SET_DISPLAY_OFFSET, 0x00,
        GFX_CMD_SET_START_LINE,
        GFX_CMD_SET_CHARGEPUMP, 0x14, // ON
        GFX_CMD_MEMORY_MODE, 0x00, // Horizontal (continuous)
        GFX_CMD_SEGMENT_REMAP | 0x0, // CA0 <-> SEG0
        GFX_CMD_COM_DIRECTION | 0x8, // DECREASING
        GFX_CMD_COM_PINS, 0x12,
        GFX_CMD_SET_CONTRAST, 0xCF,
        GFX_CMD_SET_PRECHARGE, 0xF1,
        GFX_CMD_SET_VCOM_DETECT, 0x40,
        GFX_CMD_SET_DISPLAY_ALLOW_RESUME,
        GFX_CMD_NORMAL_DISPLAY,
        GFX_CMD_DISPLAY_ON,
        GFX_CMD_SET_COLUMN_ADDRESS, 0x00, 0xFF,
        GFX_CMD_SET_PAGE_ADDRESS, 0x00, 0x07
    };

    pin_set(rst);
    runtime_delay(100000);
    pin_clear(rst);
    runtime_delay(10000000);
    pin_set(rst);

    pin_clear(dc);
//    pin_clear(cs);
    for (int i = 0; i < sizeof(cmd); i++) {
        while (u_spi_get_tx_full(SPI1));
        u_spi_buffer_write(SPI1, cmd[i]);
    }
    while (u_spi_get_busy(SPI1));
//    pin_set(cs);
    pin_set(dc);
}

void gfx_bb_set(unsigned char scol, unsigned char ecol, unsigned char spage, unsigned char epage) {
    unsigned char cmd [] = {
        GFX_CMD_SET_COLUMN_ADDRESS,
        scol,
        ecol,
        GFX_CMD_SET_PAGE_ADDRESS,
        spage,
        epage
    };

    pin_clear(dc);
//    pin_clear(cs);
    for (int i = 0 ; i < sizeof(cmd); i++) {
        while (u_spi_get_tx_full(SPI1));
        u_spi_buffer_write(SPI1, cmd[i]);
    }
    while (u_spi_get_busy(SPI1));
//    pin_set(cs);
    pin_set(dc);
}

void gfx_write(unsigned char * data, unsigned int len) {
//    pin_clear(cs);
    for (int i = 0; i < len; i++) {
        while (u_spi_get_tx_full(SPI1));
        uart_print("gfx_write...\r\n");
        u_spi_buffer_write(SPI1, data[i]);
    }
    while (u_spi_get_busy(SPI1));
//    pin_set(cs);
}

