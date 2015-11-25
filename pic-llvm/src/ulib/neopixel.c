//#include <sys/attribs.h>
#include "proc/p32mx250f128b.h"

#include "ulib/ulib.h"
#include "ulib/pins.h"
#include "ulib/neopixel.h"


/*  SCK = pin 25
 *  SDO = pin 24
 */

void neopixel_setup(void) {
    u_spi_config config = u_spi_load_config(SPI1);

    config.frame_enable = 0;
    config.master_ss_enable = 0;
    config.clock_select = 0;
    config.enhanced_buffer = 1;
    config.on = 1;
    config.disable_sdo = 0;
    config.disable_sdi = 1;
    config.mode = 0; //8 bits
    config.sample_phase = 0;
    config.cke = 0;
    config.ckp = 0;
    config.master_enable = 1;
    config.tx_int_mode = 1;

    u_spi_save_config(SPI1, config);

    u_spi_set_baud(SPI1, 0);

    u_pps_in_sdi1(PPS_IN2_RPB11);
    u_pps_out_sdo1_b(PPS_OUT3_RPB13);

    Pin ss = {PIN_GROUP_B, BITS(10)};

    pin_mode_set(ss, PIN_OUTPUT, PIN_DIGITAL);
    pin_set(ss);
/*
    u_spi_int_config iconfig = u_spi_int_load_config(SPI1);

    //iconfig.tx_enable = 1;
    iconfig.priority = 3;
    iconfig.subpriority = 0;
    iconfig.clear = 1;

    u_spi_int_save_config(SPI1, iconfig);
 */
}

void spi_rxtx(char *data, char *dest, int length) {
    Pin ss = {PIN_GROUP_B, BITS(10)};

    pin_clear(ss);

    int recieved = 0;
    int sent = 0;
    while (recieved < length) {
        if (sent < length && !u_spi_get_tx_full(SPI1)) {
            u_spi_buffer_write(SPI1, data[sent++]);
        }
        if (!u_spi_get_rx_fifo_empty(SPI1)) {
            if (dest == 0)
                recieved++;
            else
                dest[recieved++] = u_spi_buffer_read(SPI1);
        }
    }

    pin_set(ss);
}
/*
int spi_block_transfer(char *header, int header_length, char *source, unsigned int length) {
    if (block_ptr == block_end_ptr) {
        block_ptr = source;
        block_end_ptr = source + length;
        spi_rxtx(header, 0, header_length);
        IEC1SET = BITS(6);
    } else {
        return SPI_TX_FAILURE;
    }
}

void __ISR(_SPI1_VECTOR, ipl3) block_transfer_handler(void) {
    uart_print("\r\n<block transfer>\r\n");
    uart_print(tohex((int)block_ptr, 8));
    uart_print("\r\n");
    uart_print(tohex((int)block_end_ptr, 8));

    while (block_ptr != block_end_ptr && !u_spi_get_tx_full(SPI1))
        u_spi_buffer_write(SPI1, *block_ptr++);

    if (block_ptr == block_end_ptr) {
        IEC1CLR = BITS(6);
    }


    u_spi_int_clear(SPI1);
}
*/
