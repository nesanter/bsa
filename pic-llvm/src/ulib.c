/* UART */

#include "ulib/ulib.h"
#include "ulib/pins.h"
//#include "ulib/ulib_int.h"
#include "proc/p32mx250f128b.h"

/*
void u_initialize(unsigned int *errorepc) {
    u_int_setup(errorepc);
}
*/

/* ------------------------- UART ------------------------- */

u_uart_config u_uartx_load_config(u_uart_select select) {
    u_uart_config config;
    int cur_mode;
    int cur_status;

    if (select == UART1) {
        cur_mode = U1MODE;
        cur_status = U1STA;
    } else if (select == UART2) {
        cur_mode = U2MODE;
        cur_status = U2STA;
    }

    config.on = (cur_mode & BITS(15)) >> 15;
    config.stop_in_idle = (cur_mode & BITS(13)) >> 13;
    config.irda_enable = (cur_mode & BITS(12)) >> 12;
    config.rx_flow_mode = (cur_mode & BITS(11)) >> 11;
    config.flow_control_enable = (cur_mode & (BITS(9) | BITS(8))) >> 8;
    config.wake = (cur_mode & BITS(7)) >> 7;
    config.loopback = (cur_mode & BITS(6)) >> 6;
    config.auto_baud = (cur_mode & BITS(5)) >> 5;
    config.rx_invert = (cur_mode & BITS(4)) >> 4;
    config.baud_mode = (cur_mode & BITS(3)) >> 3;
    config.parity_data_mode = (cur_mode & (BITS(2) | BITS(1))) >> 1;
    config.stop_mode = (cur_mode & BITS(0));

    config.address_detect_enable = (cur_status & BITS(24)) >> 24;
    config.address = (cur_status & (0xF << 16)) >> 16;
    config.tx_int_mode = (cur_status & (BITS(15) | BITS(14))) >> 14;
    config.tx_invert = (cur_status & BITS(13)) >> 13;
    config.rx_enable = (cur_status & BITS(12)) >> 12;
    config.tx_enable = (cur_status & BITS(10)) >> 10;
    config.rx_int_mode = (cur_status & (BITS(6) | BITS(7))) >> 7;
    config.address_detect_enable = (cur_status & BITS(5)) >> 5;

    return config;
}

void u_uartx_save_config(u_uart_select select, u_uart_config config) {
    int cur_mode = 0;
    int cur_status = 0;

    cur_mode |= (config.on & 1) << 15;
    cur_mode |= (config.stop_in_idle & 1) << 13;
    cur_mode |= (config.irda_enable & 1) << 12;
    cur_mode |= (config.rx_flow_mode & 1) << 11;
    cur_mode |= (config.flow_control_enable & 3) << 8;
    cur_mode |= (config.wake & 1) << 7;
    cur_mode |= (config.loopback & 6) << 6;
    cur_mode |= (config.auto_baud & 1) << 5;
    cur_mode |= (config.rx_invert & 1) << 4;
    cur_mode |= (config.baud_mode & 1) << 3;
    cur_mode |= (config.parity_data_mode & 3) << 1;
    cur_mode |= (config.stop_mode & 1);

    cur_status |= (config.address_detect_enable & 1) << 24;
    cur_status |= (config.address & 0xF) << 16;
    cur_status |= (config.tx_int_mode & 3) << 14;
    cur_status |= (config.tx_invert & 1) << 13;
    cur_status |= (config.rx_enable & 1) << 12;
    cur_status |= (config.tx_enable & 1) << 10;
    cur_status |= (config.rx_int_mode & 3) << 7;
    cur_status |= (config.address_detect_enable & 1) << 5;

    if (select == UART1) {
        U1STA = cur_status;
        U1MODE = cur_mode;
    } else if (select == UART2) {
        U2STA = cur_status;
        U2MODE = cur_mode;
    }
}

int u_uartx_baud_set(u_uart_select select, unsigned int system_freq, unsigned int rate) {
    unsigned short baud = ((system_freq / rate) / 16);

    int error = (system_freq / (16 * baud)) - rate;

    if (baud > 0)
        baud--;

    if (select == UART1) {
        U1BRG = baud;
    } else if (select == UART2) {
        U2BRG = baud;
    }
    
    return error;
}

void u_uartx_send_break(u_uart_select select) {
    if (select == UART1) {
        U1STASET = BITS(11);
    } else if (select == UART2) {
        U2STASET = BITS(11);
    }
}

int u_uartx_get_break_status(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(11);
    } else if (select == UART2) {
        return U2STA & BITS(11);
    }
    return 0;
}

int u_uartx_get_tx_full(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(9);
    } else if (select == UART2) {
        return U2STA & BITS(9);
    }
    return 0;
}

int u_uartx_get_tx_shift_empty(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(8);
    } else if (select == UART2) {
        return U2STA & BITS(8);
    }
    return 0;
}

int u_uartx_get_rx_idle(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(4);
    } else if (select == UART2) {
        return U2STA & BITS(4);
    }
    return 0;
}

int u_uartx_get_parity_error(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(3);
    } else if (select == UART2) {
        return U2STA & BITS(3);
    }
    return 0;
}

int u_uartx_get_framing_error(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(2);
    } else if (select == UART2) {
        return U2STA & BITS(2);
    }
    return 0;
}

int u_uartx_get_overrun_error(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(1);
    } else if (select == UART2) {
        return U1STA & BITS(1);
    }
    return 0;
}

void u_uartx_clear_overrun_error(u_uart_select select) {
    if (select == UART1) {
        U1STACLR = BITS(1);
    } else if (select == UART2) {
        U1STACLR = BITS(1);
    }
}

int u_uartx_get_rx_available(u_uart_select select) {
    if (select == UART1) {
        return U1STA & BITS(0);
    } else if (select == UART2) {
        return U2STA & BITS(0);
    }
    return 0;
}

void  u_uartx_tx_register_write(u_uart_select select, char c) {
    if (select == UART1) {
        U1TXREG = c;
    } else if (select == UART2) {
        U2RXREG = c;
    }
}

char u_uartx_rx_register_read(u_uart_select select) {
    if (select == UART1) {
        return U1RXREG;
    } else if (select == UART2) {
        return U2RXREG;
    }
    return 0;
}

u_uart_int_config u_uartx_int_load_config(u_uart_select select) {
    int enable, priority;
    u_uart_int_config config;

    if (select == UART1) {
        enable = IEC1;
        priority = IPC8;

        config.error_enable = (enable & BITS(7)) >> 7;
        config.rx_enable = (enable & BITS(8)) >> 8;
        config.tx_enable = (enable & BITS(9)) >> 9;
        config.priority = (priority & (BITS(4) | BITS(3) | BITS(2))) >> 2;
        config.subpriority = (priority & (BITS(1) | BITS(0)));
        config.error_clear = 0;
        config.rx_clear = 0;
        config.tx_clear = 0;
    } else if (select == UART2) {
        enable = IEC1;
        priority = IPC9;

        config.error_enable = (enable & BITS(21)) >> 21;
        config.rx_enable = (enable & BITS(22)) >> 22;
        config.tx_enable = (enable & BITS(23)) >> 23;
        config.priority = (priority & (BITS(12) | BITS(11) | BITS(10))) >> 10;
        config.subpriority = (priority & (BITS(9) | BITS(8))) >> 8;
        config.error_clear = 0;
        config.tx_clear = 0;
        config.rx_clear = 0;
    }

    return config;
}

void u_uartx_int_save_config(u_uart_select select, u_uart_int_config config) {
    int enable, priority, flags;
    if (select == UART1) {
        enable = IEC1;
        priority = IPC8;
        flags = 0;

        enable = enable ^ (enable & (BITS(7) | BITS(8) | BITS(9)));
        priority = priority ^ (priority & (BITS(4) | BITS(3) | BITS(2) | BITS(1) | BITS(0)));

        if (config.error_enable)
            enable |= BITS(7);
        if (config.rx_enable)
            enable |= BITS(8);
        if (config.tx_enable)
            enable |= BITS(9);

        priority |= (config.priority & 0x7) << 2;
        priority |= (config.subpriority & 0x3);

        if (config.error_clear)
            flags |= BITS(7);
        if (config.rx_clear)
            flags |= BITS(8);
        if (config.tx_clear)
            flags |= BITS(9);

        IFS1CLR |= flags;
        IPC8 = priority;
        IEC1 = enable;
    } else if (select == UART2) {
        enable = IEC1;
        priority = IPC8;
        flags = 0;

        enable = enable ^ (enable & (BITS(21) | BITS(22) | BITS(23)));
        priority = priority ^ (priority & (BITS(12) | BITS(11) | BITS(10) | BITS(9) | BITS(8)));

        if (config.error_enable)
            enable |= BITS(21);
        if (config.rx_enable)
            enable |= BITS(22);
        if (config.tx_enable)
            enable |= BITS(23);

        priority |= (config.priority & 0x7) << 10;
        priority |= (config.subpriority & 0x3) << 8;

        if (config.error_clear)
            flags |= BITS(21);
        if (config.rx_clear)
            flags |= BITS(22);
        if (config.tx_clear)
            flags |= BITS(23);

        IFS1CLR |= flags;
        IPC9 = priority;
        IEC1 = enable;
    }
}

void u_uartx_int_clear(u_uart_select select) {
    if (select == UART1)
        IFS1CLR = (BITS(7) | BITS(8) | BITS(9));
    else if (select == UART2)
        IFS1CLR = (BITS(23) | BITS(22) | BITS(21));
}

/* ------------------------- PPS ------------------------- */

void u_pps_in_int4(u_pps_in_group1 pin) {
    INT4R = pin;
}
void u_pps_in_t2ck(u_pps_in_group1 pin) {
    T2CKR = pin;
}
void u_pps_in_ic4(u_pps_in_group1 pin) {
    IC4R = pin;
}
void u_pps_in_ss1(u_pps_in_group1 pin) {
    SS1R = pin;
}
void u_pps_refclki(u_pps_in_group1 pin) {
    REFCLKIR = pin;
}

void u_pps_in_int3(u_pps_in_group2 pin) {
    INT3R = pin;
}
void u_pps_in_t3ck(u_pps_in_group2 pin) {
    T3CKR = pin;
}
void u_pps_in_ic3(u_pps_in_group2 pin) {
    IC3R = pin;
}
void u_pps_in_u1cts(u_pps_in_group2 pin) {
    U1CTSR = pin;
}
void u_pps_in_u2rx(u_pps_in_group2 pin) {
    U2RXR = pin;
}
void u_pps_in_sdi1(u_pps_in_group2 pin) {
    SDI1R = pin;
}

void u_pps_in_int2(u_pps_in_group3 pin) {
    INT2R = pin;
}
void u_pps_in_t4ck(u_pps_in_group3 pin) {
    T4CKR = pin;
}
void u_pps_in_ic1(u_pps_in_group3 pin) {
    IC1R = pin;
}
void u_pps_in_ic5(u_pps_in_group3 pin) {
    IC5R = pin;
}
void u_pps_in_u1rx(u_pps_in_group3 pin) {
    U1RXR = pin;
}
void u_pps_in_u2cts(u_pps_in_group3 pin) {
    U2CTSR = pin;
}
void u_pps_in_sdi2(u_pps_in_group3 pin) {
    SDI2R = pin;
}
void u_pps_in_ocfb(u_pps_in_group3 pin) {
    OCFBR = pin;
}

void u_pps_in_int1(u_pps_in_group4 pin) {
    INT1R = pin;
}
void u_pps_in_t5ck(u_pps_in_group4 pin) {
    T5CKR = pin;
}
void u_pps_in_ic2(u_pps_in_group4 pin) {
    IC2R = pin;
}
void u_pps_in_ss2(u_pps_in_group4 pin) {
    SS2R = pin;
}
void u_pps_in_ocfa(u_pps_in_group4 pin) {
    OCFAR = pin;
}

void u_pps_out_disable1(u_pps_out_group1 pin) {
    switch (pin) {
        case PPS_OUT1_RPA0:
            RPA0R = 0;
            break;
        case PPS_OUT1_RPB3:
            RPB3R = 0;
            break;
        case PPS_OUT1_RPB4:
            RPB4R = 0;
            break;
        case PPS_OUT1_RPB15:
            RPB15R = 0;
            break;
        case PPS_OUT1_RPB7:
            RPB7R = 0;
            break;
    }
}
void u_pps_out_u1tx(u_pps_out_group1 pin) {
    switch (pin) {
        case PPS_OUT1_RPA0:
            RPA0R = 1;
            break;
        case PPS_OUT1_RPB3:
            RPB3R = 1;
            break;
        case PPS_OUT1_RPB4:
            RPB4R = 1;
            break;
        case PPS_OUT1_RPB15:
            RPB15R = 1;
            break;
        case PPS_OUT1_RPB7:
            RPB7R = 1;
            break;
    }
}
void u_pps_out_u2rts(u_pps_out_group1 pin) {
    switch (pin) {
        case PPS_OUT1_RPA0:
            RPA0R = 2;
            break;
        case PPS_OUT1_RPB3:
            RPB3R = 2;
            break;
        case PPS_OUT1_RPB4:
            RPB4R = 2;
            break;
        case PPS_OUT1_RPB15:
            RPB15R = 2;
            break;
        case PPS_OUT1_RPB7:
            RPB7R = 2;
            break;
    }
}
void u_pps_out_ss1(u_pps_out_group1 pin) {
    switch (pin) {
        case PPS_OUT1_RPA0:
            RPA0R = 3;
            break;
        case PPS_OUT1_RPB3:
            RPB3R = 3;
            break;
        case PPS_OUT1_RPB4:
            RPB4R = 3;
            break;
        case PPS_OUT1_RPB15:
            RPB15R = 3;
            break;
        case PPS_OUT1_RPB7:
            RPB7R = 3;
            break;
    }
}
void u_pps_out_oc1(u_pps_out_group1 pin) {
    switch (pin) {
        case PPS_OUT1_RPA0:
            RPA0R = 5;
            break;
        case PPS_OUT1_RPB3:
            RPB3R = 5;
            break;
        case PPS_OUT1_RPB4:
            RPB4R = 5;
            break;
        case PPS_OUT1_RPB15:
            RPB15R = 5;
            break;
        case PPS_OUT1_RPB7:
            RPB7R = 5;
            break;
    }
}
void u_pps_out_c2out(u_pps_out_group1 pin) {
    switch (pin) {
        case PPS_OUT1_RPA0:
            RPA0R = 7;
            break;
        case PPS_OUT1_RPB3:
            RPB3R = 7;
            break;
        case PPS_OUT1_RPB4:
            RPB4R = 7;
            break;
        case PPS_OUT1_RPB15:
            RPB15R = 7;
            break;
        case PPS_OUT1_RPB7:
            RPB7R = 7;
            break;
    }
}

void u_pps_out_disable2(u_pps_out_group2 pin) {
    switch (pin) {
        case PPS_OUT2_RPA1:
            RPA1R = 0;
            break;
        case PPS_OUT2_RPB5:
            RPB5R = 0;
            break;
        case PPS_OUT2_RPB1:
            RPB1R = 0;
            break;
        case PPS_OUT2_RPB11:
            RPB11R = 0;
            break;
        case PPS_OUT2_RPB8:
            RPB8R = 0;
            break;
        case PPS_OUT2_RPA8:
            RPA8R = 0;
            break;
        case PPS_OUT2_RPA9:
            RPA9R = 0;
            break;
    }
}
void u_pps_out_sdo1_a(u_pps_out_group2 pin) {
    switch (pin) {
        case PPS_OUT2_RPA1:
            RPA1R = 3;
            break;
        case PPS_OUT2_RPB5:
            RPB5R = 3;
            break;
        case PPS_OUT2_RPB1:
            RPB1R = 3;
            break;
        case PPS_OUT2_RPB11:
            RPB11R = 3;
            break;
        case PPS_OUT2_RPB8:
            RPB8R = 3;
            break;
        case PPS_OUT2_RPA8:
            RPA8R = 3;
            break;
        case PPS_OUT2_RPA9:
            RPA9R = 3;
            break;
    }
}
void u_pps_out_sdo2_a(u_pps_out_group2 pin) {
    switch (pin) {
        case PPS_OUT2_RPA1:
            RPA1R = 4;
            break;
        case PPS_OUT2_RPB5:
            RPB5R = 4;
            break;
        case PPS_OUT2_RPB1:
            RPB1R = 4;
            break;
        case PPS_OUT2_RPB11:
            RPB11R = 4;
            break;
        case PPS_OUT2_RPB8:
            RPB8R = 4;
            break;
        case PPS_OUT2_RPA8:
            RPA8R = 4;
            break;
        case PPS_OUT2_RPA9:
            RPA9R = 4;
            break;
    }
}
void u_pps_out_oc2(u_pps_out_group2 pin) {
    switch (pin) {
        case PPS_OUT2_RPA1:
            RPA1R = 5;
            break;
        case PPS_OUT2_RPB5:
            RPB5R = 5;
            break;
        case PPS_OUT2_RPB1:
            RPB1R = 5;
            break;
        case PPS_OUT2_RPB11:
            RPB11R = 5;
            break;
        case PPS_OUT2_RPB8:
            RPB8R = 5;
            break;
        case PPS_OUT2_RPA8:
            RPA8R = 5;
            break;
        case PPS_OUT2_RPA9:
            RPA9R = 5;
            break;
    }
}
void u_pps_out_c3out(u_pps_out_group2 pin) {
    switch (pin) {
        case PPS_OUT2_RPA1:
            RPA1R = 7;
            break;
        case PPS_OUT2_RPB5:
            RPB5R = 7;
            break;
        case PPS_OUT2_RPB1:
            RPB1R = 7;
            break;
        case PPS_OUT2_RPB11:
            RPB11R = 7;
            break;
        case PPS_OUT2_RPB8:
            RPB8R = 7;
            break;
        case PPS_OUT2_RPA8:
            RPA8R = 7;
            break;
        case PPS_OUT2_RPA9:
            RPA9R = 7;
            break;
    }
}

void u_pps_out_disable3(u_pps_out_group3 pin) {
    switch (pin) {
        case PPS_OUT3_RPA2:
            RPA2R = 0;
            break;
        case PPS_OUT3_RPA4:
            RPA4R = 0;
            break;
        case PPS_OUT3_RPB13:
            RPB13R = 0;
            break;
        case PPS_OUT3_RPB2:
            RPB2R = 0;
            break;
    }
}
void u_pps_out_sdo1_b(u_pps_out_group3 pin) {
    switch (pin) {
        case PPS_OUT3_RPA2:
            RPA2R = 3;
            break;
        case PPS_OUT3_RPA4:
            RPA4R = 3;
            break;
        case PPS_OUT3_RPB13:
            RPB13R = 3;
            break;
        case PPS_OUT3_RPB2:
            RPB2R = 3;
            break;
    }
}
void u_pps_out_sdo2_b(u_pps_out_group3 pin) {
    switch (pin) {
        case PPS_OUT3_RPA2:
            RPA2R = 4;
            break;
        case PPS_OUT3_RPA4:
            RPA4R = 4;
            break;
        case PPS_OUT3_RPB13:
            RPB13R = 4;
            break;
        case PPS_OUT3_RPB2:
            RPB2R = 4;
            break;
    }
}
void u_pps_out_oc4(u_pps_out_group3 pin) {
    switch (pin) {
        case PPS_OUT3_RPA2:
            RPA2R = 5;
            break;
        case PPS_OUT3_RPA4:
            RPA4R = 5;
            break;
        case PPS_OUT3_RPB13:
            RPB13R = 5;
            break;
        case PPS_OUT3_RPB2:
            RPB2R = 5;
            break;
    }
}
void u_pps_out_oc5(u_pps_out_group3 pin) {
    switch (pin) {
        case PPS_OUT3_RPA2:
            RPA2R = 6;
            break;
        case PPS_OUT3_RPA4:
            RPA4R = 6;
            break;
        case PPS_OUT3_RPB13:
            RPB13R = 6;
            break;
        case PPS_OUT3_RPB2:
            RPB2R = 6;
            break;
    }
}
void u_pps_out_refclko(u_pps_out_group3 pin) {
    switch (pin) {
        case PPS_OUT3_RPA2:
            RPA2R = 7;
            break;
        case PPS_OUT3_RPA4:
            RPA4R = 7;
            break;
        case PPS_OUT3_RPB13:
            RPB13R = 7;
            break;
        case PPS_OUT3_RPB2:
            RPB2R = 7;
            break;
    }
}

void u_pps_out_disable4(u_pps_out_group4 pin) {
    switch (pin) {
        case PPS_OUT4_RPA3:
            RPA3R = 0;
            break;
        case PPS_OUT4_RPB14:
            RPB14R = 0;
            break;
        case PPS_OUT4_RPB0:
            RPB0R = 0;
            break;
        case PPS_OUT4_RPB10:
            RPB10R = 0;
            break;
        case PPS_OUT4_RPB9:
            RPB9R = 0;
            break;
    }
}
void u_pps_out_u1rts(u_pps_out_group4 pin) {
    switch (pin) {
        case PPS_OUT4_RPA3:
            RPA3R = 1;
            break;
        case PPS_OUT4_RPB14:
            RPB14R = 1;
            break;
        case PPS_OUT4_RPB0:
            RPB0R = 1;
            break;
        case PPS_OUT4_RPB10:
            RPB10R = 1;
            break;
        case PPS_OUT4_RPB9:
            RPB9R = 1;
            break;
    }
}
void u_pps_out_u2tx(u_pps_out_group4 pin) {
    switch (pin) {
        case PPS_OUT4_RPA3:
            RPA3R = 2;
            break;
        case PPS_OUT4_RPB14:
            RPB14R = 2;
            break;
        case PPS_OUT4_RPB0:
            RPB0R = 2;
            break;
        case PPS_OUT4_RPB10:
            RPB10R = 2;
            break;
        case PPS_OUT4_RPB9:
            RPB9R = 2;
            break;
    }
}
void u_pps_out_ss2(u_pps_out_group4 pin) {
    switch (pin) {
        case PPS_OUT4_RPA3:
            RPA3R = 4;
            break;
        case PPS_OUT4_RPB14:
            RPB14R = 4;
            break;
        case PPS_OUT4_RPB0:
            RPB0R = 4;
            break;
        case PPS_OUT4_RPB10:
            RPB10R = 4;
            break;
        case PPS_OUT4_RPB9:
            RPB9R = 4;
            break;
    }
}
void u_pps_out_oc3(u_pps_out_group4 pin) {
    switch (pin) {
        case PPS_OUT4_RPA3:
            RPA3R = 5;
            break;
        case PPS_OUT4_RPB14:
            RPB14R = 5;
            break;
        case PPS_OUT4_RPB0:
            RPB0R = 5;
            break;
        case PPS_OUT4_RPB10:
            RPB10R = 5;
            break;
        case PPS_OUT4_RPB9:
            RPB9R = 5;
            break;
    }
}
void u_pps_out_c1out(u_pps_out_group4 pin) {
    switch (pin) {
        case PPS_OUT4_RPA3:
            RPA3R = 7;
            break;
        case PPS_OUT4_RPB14:
            RPB14R = 7;
            break;
        case PPS_OUT4_RPB0:
            RPB0R = 7;
            break;
        case PPS_OUT4_RPB10:
            RPB10R = 7;
            break;
        case PPS_OUT4_RPB9:
            RPB9R = 7;
            break;
    }
}

/* ------------------------- TIMER ------------------------- */

u_timerb_config u_timerb_load_config(u_timerb_select select) {
    u_timerb_config config;
    int con, sidl;

    switch (select) {
        case TIMER2:
            con = T2CON;
            sidl = con;
            break;
        case TIMER3:
            con = T3CON;
            sidl = con;
            break;
        case TIMER23:
            con = T2CON;
            sidl = T3CON;
            break;
        case TIMER4:
            con = T4CON;
            sidl = con;
        case TIMER5:
            con = T5CON;
            sidl = con;
        case TIMER45:
            con = T4CON;
            sidl = T5CON;
            break;
    }

    config.on = (con & BITS(15)) >> 15;
    config.stop_in_idle = (sidl & BITS(13)) >> 13;
    config.gated_acc_enable = (con & BITS(7)) >> 7;
    config.prescaler = (con & (BITS(6) | BITS(5) | BITS(4))) >> 4;
    config.t32_enable = (con & BITS(3)) >> 3;
    config.source = (con & BITS(1)) >> 1;

    return config;
}

void u_timerb_save_config(u_timerb_select select, u_timerb_config config) {
    int con = 0, sidl = 0;

    con |= (config.on & 1) << 15;
    con |= (config.stop_in_idle & 1) << 13;
    con |= (config.gated_acc_enable & 1) << 7;
    con |= (config.prescaler & 7) << 4;
    con |= (config.t32_enable & 1) << 3;
    con |= (config.source & 1) << 1;

    switch (select) {
        case TIMER2:
            T2CON = con;
            break;
        case TIMER3:
            T3CON = con;
            break;
        case TIMER23:
            T2CON = con;
            if (config.stop_in_idle)
                T3CONSET = BITS(13);
            else
                T3CONCLR = BITS(13);
            break;
        case TIMER4:
            T4CON = con;
            break;
        case TIMER5:
            T5CON = con;
            break;
        case TIMER45:
            T4CON = con;
            if (config.stop_in_idle)
                T5CONSET = BITS(13);
            else
                T5CONCLR = BITS(13);
            break;
    }
}

unsigned int u_timerb_read(u_timerb_select select) {
    switch (select) {
        case TIMER2:
            return TMR2;
        case TIMER3:
            return TMR3;
        case TIMER23:
            return TMR2 | (TMR3 << 16);
        case TIMER4:
            return TMR4;
        case TIMER5:
            return TMR5;
        case TIMER45:
            return TMR4 | (TMR5 << 16);
    }
}

unsigned int u_timerb_period_read(u_timerb_select select) {
    switch (select) {
        case TIMER2:
            return PR2;
        case TIMER3:
            return PR3;
        case TIMER23:
            return PR2 | (PR3 << 16);
        case TIMER4:
            return PR4;
        case TIMER5:
            return PR5;
        case TIMER45:
            return PR4 | (PR5 << 16);
    }
}

void u_timerb_write(u_timerb_select select, unsigned int period) {
    switch (select) {
        case TIMER2:
            TMR2 = period & 0xFFFF;
            break;
        case TIMER3:
            TMR3 = period & 0xFFFF;
            break;
        case TIMER23:
            TMR2 = period & 0xFFFF;
            TMR3 = (period & 0xFFFF0000) >> 16;
            break;
        case TIMER4:
            TMR4 = period & 0xFFFF;
            break;
        case TIMER5:
            TMR5 = period & 0xFFFF;
            break;
        case TIMER45:
            TMR4 = period & 0xFFFF;
            TMR5 = (period & 0xFFFF0000) >> 16;
            break;
    }
}

void u_timerb_period_write(u_timerb_select select, unsigned int period) {
    switch (select) {
        case TIMER2:
            PR2 = period & 0xFFFF;
            break;
        case TIMER3:
            PR3 = period & 0xFFFF;
            break;
        case TIMER23:
            PR2 = period & 0xFFFF;
            PR3 = (period & 0xFFFF0000) >> 16;
            break;
        case TIMER4:
            PR4 = period & 0xFFFF;
            break;
        case TIMER5:
            PR5 = period & 0xFFFF;
            break;
        case TIMER45:
            PR4 = period & 0xFFFF;
            PR5 = (period & 0xFFFF0000) >> 16;
            break;
    }
}

u_timerb_int_config u_timerb_int_load_config(u_timerb_select select) {
    u_timerb_int_config config;

    switch (select) {
        case TIMER2:
            config.enable = (IEC0 & BITS(9)) >> 9;
            config.priority = (IPC2 & (BITS(4) | BITS(3) | BITS(2))) >> 2;
            config.subpriority = (IPC2 & (BITS(1) | BITS(0)));
            break;
        case TIMER3:
        case TIMER23:
            config.enable = (IEC0 & BITS(14)) >> 14;
            config.priority = (IPC3 & (BITS(4) | BITS(3) | BITS(2))) >> 2;
            config.subpriority = (IPC3 & (BITS(1) | BITS(0)));
            break;
        case TIMER4:
            config.enable = (IEC0 & BITS(19)) >> 19;
            config.priority = (IPC4 & (BITS(4) | BITS(3) | BITS(2))) >> 2;
            config.subpriority = (IPC4 & (BITS(1) | BITS(0)));
            break;
        case TIMER5:
        case TIMER45:
            config.enable = (IEC0 & BITS(24)) >> 24;
            config.priority = (IPC5 & (BITS(4) | BITS(3) | BITS(2))) >> 2;
            config.subpriority = (IPC5 & (BITS(1) | BITS(0)));
            break;
    }
    config.clear = 0;

    return config;
}

void u_timerb_int_save_config(u_timerb_select select, u_timerb_int_config config) {
    switch (select) {
        case TIMER2:
            IPC2CLR = BITS(4) | BITS(3) | BITS(2) | BITS(1) | BITS(0);
            IPC2SET = (config.priority << 2) | config.subpriority;

            if (config.clear)
                IFS0CLR = BITS(9);

            if (config.enable)
                IEC0SET = BITS(9);
            else
                IEC0CLR = BITS(9);
            break;
        case TIMER3:
        case TIMER23:
            IPC3CLR = BITS(4) | BITS(3) | BITS(2) | BITS(1) | BITS(0);
            IPC3SET = (config.priority << 2) | config.subpriority;

            if (config.clear)
                IFS0CLR = BITS(14);

            if (config.enable)
                IEC0SET = BITS(14);
            else
                IEC0CLR = BITS(14);
            break;
        case TIMER4:
            IPC4CLR = BITS(4) | BITS(3) | BITS(2) | BITS(1) | BITS(0);
            IPC4SET = (config.priority << 2) | config.subpriority;

            if (config.clear)
                IFS0CLR = BITS(19);

            if (config.enable)
                IEC0SET = BITS(19);
            else
                IEC0CLR = BITS(19);
            break;
        case TIMER5:
        case TIMER45:
            IPC5CLR = BITS(4) | BITS(3) | BITS(2) | BITS(1) | BITS(0);
            IPC5SET = (config.priority << 2) | config.subpriority;

            if (config.clear)
                IFS0CLR = BITS(24);

            if (config.enable)
                IEC0SET = BITS(24);
            else
                IEC0CLR = BITS(24);
            break;
    }
}

void u_timerb_int_clear(u_timerb_select select) {
    switch (select) {
        case TIMER2:
        case TIMER23:
            IFS0CLR = BITS(9);
        case TIMER3:
            IFS0CLR = BITS(14);
            break;
        case TIMER4:
        case TIMER45:
            IFS0CLR = BITS(19);
            break;
        case TIMER5:
            IFS0CLR = BITS(24);
            break;
    }
}

/* ------------------------- SPI ------------------------- */

u_spi_config u_spi_load_config(u_spi_select select) {
    u_spi_config config;
    int con, con2;

    switch (select) {
        case SPI1:
            con = SPI1CON;
            con2 = SPI1CON2;
            break;
        case SPI2:
            con = SPI2CON;
            con2 = SPI2CON2;
            break;
    }

    config.frame_enable = (con & BITS(31)) >> 31;
    config.frame_direction = (con & BITS(30)) >> 30;
    config.ss_polarity = (con & BITS(29)) >> 29;
    config.master_ss_enable = (con & BITS(28)) >> 28;
    config.frame_width = (con & BITS(27)) >> 27;
    config.frame_counter = (con & (BITS(26) | BITS(25) | BITS(24))) >> 24;
    config.clock_select = (con & BITS(23)) >> 23;
    config.frame_edge = (con & BITS(17)) >> 17;
    config.enhanced_buffer = (con & BITS(16)) >> 16;
    config.on = (con & BITS(15)) >> 15;
    config.stop_in_idle = (con & BITS(13)) >> 13;
    config.disable_sdo = (con & BITS(12)) >> 12;
    config.mode = (con & (BITS(11) | BITS(10))) >> 10;
    config.sample_phase = (con & BITS(9)) >> 9;
    config.cke = (con & BITS(8)) >> 8;
    config.slave_ss_enable = (con & BITS(7)) >> 7;
    config.ckp = (con & BITS(6)) >> 6;
    config.master_enable = (con & BITS(5)) >> 5;
    config.disable_sdi = (con & BITS(4)) >> 4;
    config.tx_int_mode = (con & (BITS(3) | BITS(2))) >> 2;
    config.rx_int_mode = (con & (BITS(1) | BITS(0)));

    config.sign_extend = (con2 & BITS(15)) >> 15;
    config.frame_error_is_int = (con2 & BITS(12)) >> 12;
    config.overflow_is_int = (con2 & BITS(11)) >> 11;
    config.underrun_is_int = (con2 & BITS(10)) >> 10;
    config.audio_overflow_ignore = (con2 & BITS(9)) >> 9;
    config.audio_underrun_ignore = (con2 & BITS(8)) >> 8;
    config.audio_enable = (con2 & BITS(7)) >> 7;
    config.audio_format = (con2 & BITS(3)) >> 3;
    config.audio_protocol = (con2 & (BITS(1) | BITS(0)));

    return config;
}

void u_spi_save_config(u_spi_select select, u_spi_config config) {
    int con = 0;
    int con2 = 0;

    con |= (config.frame_enable & 1) << 31;
    con |= (config.frame_direction & 1) << 30;
    con |= (config.ss_polarity & 1) << 29;
    con |= (config.master_ss_enable & 1) << 28;
    con |= (config.frame_width & 1) << 27;
    con |= (config.frame_counter & 7) << 24;
    con |= (config.clock_select & 1) << 23;
    con |= (config.frame_edge & 1) << 17;
    con |= (config.enhanced_buffer & 1) << 16;
    con |= (config.on & 1) << 15;
    con |= (config.stop_in_idle & 1) << 13;
    con |= (config.disable_sdo & 1) << 12;
    con |= (config.mode & 1) << 10;
    con |= (config.sample_phase & 1) << 9;
    con |= (config.cke & 1) << 8;
    con |= (config.slave_ss_enable & 1) << 7;
    con |= (config.ckp & 1) << 6;
    con |= (config.master_enable & 1) << 5;
    con |= (config.disable_sdi & 1) << 4;
    con |= (config.tx_int_mode & 3) << 2;
    con |= (config.rx_int_mode & 3);

    con2 |= (config.sign_extend & 1) << 15;
    con2 |= (config.frame_error_is_int & 1) << 12;
    con2 |= (config.overflow_is_int & 1) << 11;
    con2 |= (config.underrun_is_int & 1) << 10;
    con2 |= (config.audio_overflow_ignore & 1) << 9;
    con2 |= (config.audio_underrun_ignore & 1) << 8;
    con2 |= (config.audio_enable & 1) << 7;
    con2 |= (config.audio_format & 1) << 3;
    con2 |= (config.audio_protocol & 3);

    switch (select) {
        case SPI1:
            SPI1CON = con;
            SPI1CON2 = con2;
            break;
        case SPI2:
            SPI2CON = con;
            SPI2CON2 = con2;
            break;
    }
}

int u_spi_get_rx_elements(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & (BITS(28) | BITS(27) | BITS(26) | BITS(25) | BITS(24))) >> 24;
        case SPI2:
            return (SPI2STAT & (BITS(28) | BITS(27) | BITS(26) | BITS(25) | BITS(24))) >> 24;
    }
}
int u_spi_get_tx_elements(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & (BITS(20) | BITS(19) | BITS(18) | BITS(17) | BITS(16))) >> 16;
        case SPI2:
            return (SPI2STAT & (BITS(20) | BITS(19) | BITS(18) | BITS(17) | BITS(16))) >> 16;
    }
}

int u_spi_get_frame_error(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(12)) >> 12;
        case SPI2:
            return (SPI2STAT & BITS(12)) >> 12;
    }
}
void u_spi_clear_frame_error(u_spi_select select) {
    switch (select) {
        case SPI1:
            SPI1STATCLR = BITS(12);
            break;
        case SPI2:
            SPI2STATCLR = BITS(12);
            break;
    }
}
int u_spi_get_busy(u_spi_select select){
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(11)) >> 11;
        case SPI2:
            return (SPI2STAT & BITS(11)) >> 11;
    }
}
int u_spi_get_tx_underrun(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(8)) >> 8;
        case SPI2:
            return (SPI2STAT & BITS(8)) >> 8;
    }
}
void u_spi_clear_tx_underrun(u_spi_select select) {
    switch (select) {
        case SPI1:
            SPI1STATCLR = BITS(8);
            break;
        case SPI2:
            SPI2STATCLR = BITS(8);
            break;
    }
}
int u_spi_get_shift_empty(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(7)) >> 7;
        case SPI2:
            return (SPI2STAT & BITS(7)) >> 7;
    }
}
int u_spi_get_rx_overflow(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(6)) >> 6;
        case SPI2:
            return (SPI2STAT & BITS(6)) >> 6;
    }
}
void u_spi_clear_rx_overflow(u_spi_select select) {
    switch (select) {
        case SPI1:
            SPI1STATCLR = BITS(6);
            break;
        case SPI2:
            SPI2STATCLR = BITS(6);
            break;
    }
}
int u_spi_get_rx_fifo_empty(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(5)) >> 5;
        case SPI2:
            return (SPI2STAT & BITS(5)) >> 5;
    }
}
int u_spi_get_tx_empty(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(3)) >> 3;
        case SPI2:
            return (SPI2STAT & BITS(3)) >> 3;
    }
}
int u_spi_get_tx_full(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(1)) >> 1;
        case SPI2:
            return (SPI2STAT & BITS(1)) >> 1;
    }
}
int u_spi_get_rx_full(u_spi_select select) {
    switch (select) {
        case SPI1:
            return (SPI1STAT & BITS(0)) >> 0;
        case SPI2:
            return (SPI2STAT & BITS(0)) >> 0;
    }
}

void u_spi_buffer_write(u_spi_select select, int data) {
    switch (select) {
        case SPI1:
            SPI1BUF = data;
            break;
        case SPI2:
            SPI2BUF = data;
            break;
    }
}
int u_spi_buffer_read(u_spi_select select) {
    switch (select) {
        case SPI1:
            return SPI1BUF;
            break;
        case SPI2:
            return SPI2BUF;
    }
}

int u_spi_get_baud(u_spi_select select) {
    switch (select) {
        case SPI1:
            return SPI1BRG;
        case SPI2:
            return SPI2BRG;
    }
}
void u_spi_set_baud(u_spi_select select, int baud) {
    switch (select) {
        case SPI1:
            SPI1BRG = baud;
        case SPI2:
            SPI2BRG = baud;
    }
}

u_spi_int_config u_spi_int_load_config(u_spi_select select) {
    u_spi_int_config config;

    config.clear = 0;

    switch (select) {
        case SPI1:
            config.error_enable = (IEC1 & BITS(4)) >> 4;
            config.rx_enable = (IEC1 & BITS(5)) >> 5;
            config.tx_enable = (IEC1 & BITS(6)) >> 6;
            config.priority = (IPC7 & (BITS(28) | BITS(27) | BITS(26))) >> 26;
            config.subpriority = (IPC7 & (BITS(25) | BITS(24))) >> 24;
            break;
        case SPI2:
            config.error_enable = (IEC1 & BITS(18)) >> 18;
            config.rx_enable = (IEC1 & BITS(19)) >> 19;
            config.tx_enable = (IEC1 & BITS(20)) >> 20;
            config.priority = (IPC9 & (BITS(4) | BITS(3) | BITS(2))) >> 2;
            config.subpriority = (IPC9 & (BITS(1) | BITS(0)));
            break;
    }

    return config;
}

void u_spi_int_save_config(u_spi_select select, u_spi_int_config config) {
    switch (select) {
        case SPI1:
            IPC7CLR = BITS(28) | BITS(27) | BITS(26) | BITS(25) | BITS(24);
            IPC7SET = ((config.priority & 0x7) << 26) | ((config.subpriority & 3) << 24);

            if (config.clear)
                IFS1CLR = BITS(4) | BITS(5) | BITS(6);

            IEC1CLR = BITS(4) | BITS(5) | BITS(6);
            IEC1SET = ((config.error_enable & 1) << 4) | ((config.rx_enable & 1) << 5) | ((config.tx_enable & 1) << 6);
            break;
        case SPI2:
            IPC9CLR = BITS(4) | BITS(3) | BITS(2) | BITS(1) | BITS(0);
            IPC9SET = ((config.priority & 0x7) << 2) | (config.subpriority & 3);

            if (config.clear)
                IFS1CLR = BITS(18) | BITS(19) | BITS(20);

            IEC1CLR = BITS(18) | BITS(19) | BITS(20);
            IEC1SET = ((config.error_enable & 1) << 18) | ((config.rx_enable & 1) << 19) | ((config.tx_enable & 1) << 20);
            break;
    }
}

void u_spi_int_clear(u_spi_select select) {
    switch (select) {
        case SPI1:
            IFS1CLR = BITS(4) | BITS(5) | BITS(6);
            break;
        case SPI2:
            IFS1CLR = BITS(18) | BITS(19) | BITS(20);
            break;
    }
}

/* ------------------------- CN ------------------------- */

u_cn_config u_cn_load_config(u_cn_select select) {
    unsigned int con;
    switch (select) {
        case CNA:
            con = CNCONA;
            break;
        case CNB:
            con = CNCONB;
            break;
    }

    u_cn_config config;
    config.on = (con & BITS(15)) >> 15;
    config.stop_in_idle = (con & BITS(13)) >> 13;

    return config;
}

void u_cn_save_config(u_cn_select select, u_cn_config config) {
    unsigned int con;
    con |= (config.on & 1) << 15;
    con |= (config.stop_in_idle & 1) << 13;
    switch (select) {
        case CNA:
            CNCONA = con;
            break;
        case CNB:
            CNCONB = con;
            break;
    }
}

void u_cn_enable(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            CNENASET = p.pin;
            break;
        case PIN_GROUP_B:
            CNENBSET = p.pin;
            break;
    }
}

void u_cn_disable(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            CNENACLR = p.pin;
            break;
        case PIN_GROUP_B:
            CNENBCLR = p.pin;
            break;
    }
}

int u_cn_changed(u_cn_select select) {
    switch (select) {
        case CNA:
            return CNSTATA;
        case CNB:
            return CNSTATB;
    }
}

/* ------------------------- I2C ------------------------- */

u_i2c_config u_i2c_load_config(u_i2c_select select) {
  int cur_config;
  u_i2c_config config;
 
  switch (select) {
    case I2C1:
        cur_config = I2C1CON;
      break;

    case I2C2:
       cur_config = I2C2CON;
      break;
  }

  config.on = (cur_config & BITS(15)) >> 15;
  config.stop_in_idle = (cur_config & BITS(13)) >> 13;
  config.scl_release_control = (cur_config & BITS(12)) >> 12;
  config.strict_reserved_address_rule_enable = (cur_config & BITS(11)) >> 11;
  config.slave_address = (cur_config & BITS(10)) >> 10;
  config.slew_rate_control_disable = (cur_config & BITS(9)) >> 9;
  config.smbus_input_levels_disable = (cur_config & BITS(8)) >> 8;
  config.general_call_enable = (cur_config & BITS(7)) >> 7;
  config.scl_clock_stretch_enable = (cur_config & BITS(6)) >> 6;
  config.ack_data = (cur_config & BITS(5)) >> 5;
  config.ack_sequence_enable = (cur_config & BITS(4)) >> 4;
  config.receive_enable = (cur_config & BITS(3)) >> 3;
  config.stop_condition_enable = (cur_config & BITS(2)) >> 2;
  config.restart_condition_enable = (cur_config & BITS(1)) >> 1;
  config.start_condition_enable = (cur_config & BITS(0)) >> 0;
     
  return config;
}

void u_i2c_save_config(u_i2c_select select, u_i2c_config config) {
  int new_config;

  new_config |= (config.on & 1) << 15;
  new_config |= (config.stop_in_idle & 1) << 13;
  new_config |= (config.scl_release_control & 1) << 12;
  new_config |= (config.strict_reserved_address_rule_enable & 1) << 11;
  new_config |= (config.slave_address & 1) << 10;
  new_config |= (config.slew_rate_control_disable & 1) << 9;
  new_config |= (config.smbus_input_levels_disable & 1) << 8;
  new_config |= (config.general_call_enable & 1) << 7;
  new_config |= (config.scl_clock_stretch_enable & 1) << 6;
  new_config |= (config.ack_data & 1) << 5;
  new_config |= (config.ack_sequence_enable & 1) << 4;
  new_config |= (config.receive_enable & 1) << 3;
  new_config |= (config.stop_condition_enable & 1) << 2;
  new_config |= (config.restart_condition_enable & 1) << 1;
  new_config |= (config.start_condition_enable & 1) << 0;

  switch (select) {
    case I2C1:
      I2C1CON = new_config;
      break;

    case I2C2:
      I2C2CON = new_config;
      break;
  }
}

int u_i2c_get_acknowledge_status(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(15);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(15);
  }
  return 0;
}

int u_i2c_get_transmit_status(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(14);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(14);
  }
  return 0;
}

int u_i2c_get_master_bus_collision_detect(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(10);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(10);
  }
  return 0;
}

int u_i2c_get_general_call_status(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(9);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(9);
  }
  return 0;
}

int u_i2c_get_address_status(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(8);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(8);
  }
  return 0;
}

int u_i2c_get_write_collision_detect(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(7);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(7);
  }
  return 0;
}

int u_i2c_get_receive_overflow_status(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(6);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(6);
  }
  return 0;
}

int u_i2c_get_data_address(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(5);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(5);
  }
  return 0;
}

int u_i2c_get_stop(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(4);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(4);
  }
  return 0;
}

int u_i2c_get_start(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(3);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(3);
  }
  return 0;
}

int u_i2c_get_read_write_information(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(2);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(2);
  }
  return 0;
}

int u_i2c_get_receive_buffer_full_status(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(1);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(1);
  }
  return 0;
}

int u_i2c_get_transmit_buffer_full_status(u_i2c_select select) {
  if (select == I2C1) {
     return I2C1STAT & BITS(0);
  } else if (select == I2C2) {
     return I2C2STAT & BITS(0);
  }
  return 0;
}

void u_i2c_slave_address_register_write(u_i2c_select select, int n) {
    if (select == I2C1) {
        I2C1ADD = n;
    } else if (select == I2C2) {
        I2C2ADD = n;
    }
}

int u_i2c_slave_address_register_read(u_i2c_select select) {
    if (select == I2C1) {
        return I2C1ADD;
    } else if (select == I2C2) {
        return I2C2ADD;
    }
    return 0;
}

void u_i2c_address_mask_register_write(u_i2c_select select, int n) {
    if (select == I2C1) {
        I2C1MSK = n;
    } else if (select == I2C2) {
        I2C2MSK = n;
    }
}

int u_i2c_address_mask_register_read(u_i2c_select select) {
    if (select == I2C1) {
        return I2C1MSK;
    } else if (select == I2C2) {
        return I2C2MSK;
    }
    return 0;
}

void u_i2c_baud_rate_generator_register_write(u_i2c_select select, int n) {
    if (select == I2C1) {
        I2C1BRG = n;
    } else if (select == I2C2) {
        I2C2BRG = n;
    }
}

int u_i2c_baud_rate_generator_register_read(u_i2c_select select) {
    if (select == I2C1) {
        return I2C1BRG;
    } else if (select == I2C2) {
        return I2C2BRG;
    }
    return 0;
}

void  u_i2c_tx_register_write(u_i2c_select select, char c) {
    if (select == I2C1) {
        I2C1TRN = c;
    } else if (select == I2C2) {
        I2C2TRN = c;
    }
}

char u_i2c_rx_register_read(u_uart_select select) {
    if (select == I2C1) {
        return I2C1RCV;
    } else if (select == I2C2) {
        return I2C2RCV;
    }
    return 0;
}

