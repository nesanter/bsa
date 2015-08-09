/* 
 * File:   ulib.h
 * Author: noah
 *
 * Created on November 7, 2014, 5:50 PM
 */

#ifndef ULIB_H
#define	ULIB_H

#define BITS(n) (1L << n)

/* ------------------------- UART ------------------------- */

typedef enum {
    UART1, UART2
} u_uart_select;

typedef struct {
    char    on,
            stop_in_idle,
            irda_enable,
            rx_flow_mode,
            flow_control_enable,
            wake,
            loopback,
            auto_baud,
            rx_invert,
            baud_mode,
            parity_data_mode,
            stop_mode;
    char    address_detect_enable,
            address,
            tx_int_mode,
            tx_invert,
            rx_enable,
            tx_enable,
            rx_int_mode,
            address_detect_bit;
} u_uart_config;

u_uart_config u_uartx_load_config(u_uart_select select);
void u_uartx_save_config(u_uart_select select, u_uart_config config);

int u_uartx_baud_set(u_uart_select select, unsigned int system_freq, unsigned int rate);

void u_uartx_send_break(u_uart_select select);
int u_uartx_get_break_status(u_uart_select select);
int u_uartx_get_tx_full(u_uart_select select);
int u_uartx_get_tx_shift_empty(u_uart_select select);
int u_uartx_get_rx_idle(u_uart_select select);
int u_uartx_get_parity_error(u_uart_select select);
int u_uartx_get_framing_error(u_uart_select select);
int u_uartx_get_overrun_error(u_uart_select select);
void u_uartx_clear_overrun_error(u_uart_select select);
int u_uartx_get_rx_available(u_uart_select select);
void  u_uartx_tx_register_write(u_uart_select select, char c);
char u_uartx_rx_register_read(u_uart_select select);

typedef struct {
    char    error_enable,
            rx_enable,
            tx_enable,
            priority,
            subpriority,
            error_clear,
            rx_clear,
            tx_clear;
} u_uart_int_config;

u_uart_int_config u_uartx_int_load_config(u_uart_select select);
void u_uartx_int_save_config(u_uart_select select, u_uart_int_config config);
void u_uartx_int_clear(u_uart_select select);

/* ------------------------- PPS ------------------------- */

typedef enum {
    PPS_IN1_RPA0, PPS_IN1_RPB3, PPS1_IN_RPB4, PPS1_IN_RPB15, PPS1_IN_RPB7
} u_pps_in_group1;

typedef enum {
    PPS_IN2_RPA1, PPS_IN2_RPB5, PPS_IN2_RPB1, PPS_IN2_RPB11, PPS_IN2_RPB8
} u_pps_in_group2;

typedef enum {
    PPS_IN3_RPA2, PPS_IN3_RPB6, PPS_IN3_RPA4, PPS_IN3_RPB13, PPS_IN3_RPB2
} u_pps_in_group3;

typedef enum {
    PPS_IN4_RPA4, PPS_IN4_RPB14, PPS_IN4_RPB0, PPS_IN4_RPB10, PPS_IN4_RPB9
} u_pps_in_group4;

void u_pps_in_int4(u_pps_in_group1 pin);
void u_pps_in_t2ck(u_pps_in_group1 pin);
void u_pps_in_ic4(u_pps_in_group1 pin);
void u_pps_in_ss1(u_pps_in_group1 pin);
void u_pps_refclki(u_pps_in_group1 pin);

void u_pps_in_int3(u_pps_in_group2 pin);
void u_pps_in_t3ck(u_pps_in_group2 pin);
void u_pps_in_ic3(u_pps_in_group2 pin);
void u_pps_in_u1cts(u_pps_in_group2 pin);
void u_pps_in_u2rx(u_pps_in_group2 pin);
void u_pps_in_sdi1(u_pps_in_group2 pin);

void u_pps_in_int2(u_pps_in_group3 pin);
void u_pps_in_t4ck(u_pps_in_group3 pin);
void u_pps_in_ic1(u_pps_in_group3 pin);
void u_pps_in_ic5(u_pps_in_group3 pin);
void u_pps_in_u1rx(u_pps_in_group3 pin);
void u_pps_in_u2cts(u_pps_in_group3 pin);
void u_pps_in_sdi2(u_pps_in_group3 pin);
void u_pps_in_ocfb(u_pps_in_group3 pin);

void u_pps_in_int1(u_pps_in_group4 pin);
void u_pps_in_t5ck(u_pps_in_group4 pin);
void u_pps_in_ic2(u_pps_in_group4 pin);
void u_pps_in_ss2(u_pps_in_group4 pin);
void u_pps_in_ocfa(u_pps_in_group4 pin);

typedef enum {
    PPS_OUT1_RPA0, PPS_OUT1_RPB3, PPS_OUT1_RPB4, PPS_OUT1_RPB15, PPS_OUT1_RPB7
} u_pps_out_group1;

typedef enum {
    PPS_OUT2_RPA1, PPS_OUT2_RPB5, PPS_OUT2_RPB1, PPS_OUT2_RPB11, PPS_OUT2_RPB8,
    PPS_OUT2_RPA8, PPS_OUT2_RPA9
} u_pps_out_group2;

typedef enum {
    PPS_OUT3_RPA2, PPS_OUT3_RPA4, PPS_OUT3_RPB13, PPS_OUT3_RPB2
} u_pps_out_group3;

typedef enum {
    PPS_OUT4_RPA3, PPS_OUT4_RPB14, PPS_OUT4_RPB0, PPS_OUT4_RPB10, PPS_OUT4_RPB9
} u_pps_out_group4;

void u_pps_out_disable1(u_pps_out_group1 pin);
void u_pps_out_u1tx(u_pps_out_group1 pin);
void u_pps_out_u2rts(u_pps_out_group1 pin);
void u_pps_out_ss1(u_pps_out_group1 pin);
void u_pps_out_oc1(u_pps_out_group1 pin);
void u_pps_out_c2out(u_pps_out_group1 pin);

void u_pps_out_disable2(u_pps_out_group2 pin);
void u_pps_out_sdo1_a(u_pps_out_group2 pin);
void u_pps_out_sdo2_a(u_pps_out_group2 pin);
void u_pps_out_oc2(u_pps_out_group2 pin);
void u_pps_out_c3out(u_pps_out_group2 pin);

void u_pps_out_disable3(u_pps_out_group3 pin);
void u_pps_out_sdo1_b(u_pps_out_group3 pin);
void u_pps_out_sdo2_b(u_pps_out_group3 pin);
void u_pps_out_oc4(u_pps_out_group3 pin);
void u_pps_out_oc5(u_pps_out_group3 pin);
void u_pps_out_refclko(u_pps_out_group3 pin);

void u_pps_out_disable4(u_pps_out_group4 pin);
void u_pps_out_u1rts(u_pps_out_group4 pin);
void u_pps_out_u2tx(u_pps_out_group4 pin);
void u_pps_out_ss2(u_pps_out_group4 pin);
void u_pps_out_oc3(u_pps_out_group4 pin);
void u_pps_out_c1out(u_pps_out_group4 pin);

/* ------------------------- TIMER ------------------------- */

typedef struct {
    char    on,
            stop_in_idle,
            gated_acc_enable,
            prescaler,
            t32_enable,
            source;
} u_timerb_config;

typedef struct {
    char    enable,
            priority,
            subpriority,
            clear;
} u_timerb_int_config;

typedef enum {
    TIMER2, TIMER3, TIMER4, TIMER5,
    TIMER23, TIMER45
} u_timerb_select;

u_timerb_config u_timerb_load_config(u_timerb_select select);
void u_timerb_save_config(u_timerb_select select, u_timerb_config config);

void u_timerb_write(u_timerb_select select, unsigned int period);
unsigned int u_timerb_read(u_timerb_select select);
unsigned int u_timerb_period_read(u_timerb_select select);
void u_timerb_period_write(u_timerb_select select, unsigned int period);

u_timerb_int_config u_timerb_int_load_config(u_timerb_select select);
void u_timerb_int_save_config(u_timerb_select select, u_timerb_int_config config);

void u_timerb_int_clear(u_timerb_select select);

/* ------------------------- SPI ------------------------- */

typedef struct {
    char    frame_enable,
            frame_direction,
            ss_polarity,
            master_ss_enable,
            frame_width,
            frame_counter,
            clock_select,
            frame_edge,
            enhanced_buffer,
            on,
            stop_in_idle,
            disable_sdo,
            mode,
            sample_phase,
            cke,
            slave_ss_enable,
            ckp,
            master_enable,
            disable_sdi,
            tx_int_mode,
            rx_int_mode;
    char    sign_extend,
            frame_error_is_int,
            overflow_is_int,
            underrun_is_int,
            audio_overflow_ignore,
            audio_underrun_ignore,
            audio_enable,
            audio_format,
            audio_protocol;
} u_spi_config;

typedef struct {
    char    error_enable,
            rx_enable,
            tx_enable,
            priority,
            subpriority,
            clear;
} u_spi_int_config;

typedef enum {
    SPI1, SPI2
} u_spi_select;

u_spi_config u_spi_load_config(u_spi_select select);
void u_spi_save_config(u_spi_select select, u_spi_config config);

int u_spi_get_rx_elements(u_spi_select select);
int u_spi_get_tx_elements(u_spi_select select);
int u_spi_get_frame_error(u_spi_select select);
void u_spi_clear_frame_error(u_spi_select select);
int u_spi_get_busy(u_spi_select select);
int u_spi_get_tx_underrun(u_spi_select select);
void u_spi_clear_tx_underrun(u_spi_select select);
int u_spi_get_shift_empty(u_spi_select select);
int u_spi_get_rx_overflow(u_spi_select select);
void u_spi_clear_rx_overflow(u_spi_select select);
int u_spi_get_rx_fifo_empty(u_spi_select select);
int u_spi_get_tx_empty(u_spi_select select);
int u_spi_get_tx_full(u_spi_select select);
int u_spi_get_rx_full(u_spi_select select);

void u_spi_buffer_write(u_spi_select select, int data);
int u_spi_buffer_read(u_spi_select select);

int u_spi_get_baud(u_spi_select select);
void u_spi_set_baud(u_spi_select select, int baud);

u_spi_int_config u_spi_int_load_config(u_spi_select select);
void u_spi_int_save_config(u_spi_select, u_spi_int_config config);

void u_spi_int_clear(u_spi_select select);

/* ------------------------- CN ------------------------- */

typedef enum {
    CNA, CNB
} u_cn_select;

typedef struct {
    char    on,
            stop_in_idle;
} u_cn_config;

u_cn_config u_cn_load_config(u_cn_select select);
void u_cn_save_config(u_cn_select select, u_cn_config config);
void u_cn_enable(Pin p);
void u_cn_disable(Pin p);
int u_cn_changed(Pin p);

#endif	/* ULIB_H */

