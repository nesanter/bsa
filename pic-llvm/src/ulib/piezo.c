#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/piezo.h"

// The piezo operates from 2kHz to 10kHz
// If the peripheral clock runs at 40mHz,
// a 2kHz square wave means a 10k compare, and
// a 10kHz square wave means a 2k compare
//   (Fout = PB / PR / 2)

// OCR3 is set to 2000

void init_piezo() {
    uart_print("here\r\n");
    u_oc_config config;

    config = u_oc_load_config(OC3);
    config.mode_select = 3;
    u_oc_save_config(OC3, config);
    u_pps_out_oc3(PPS_OUT4_RPB0);
    
    u_oc_set_compare(OC3, 2000); // Creates max frequency of 10kHz

    Pin pin = {PIN_GROUP_B, BITS(0)};
    pin_mode_set(pin, 0, 0);

    u_timerb_config tconfig = u_timerb_load_config(TIMER2);
    tconfig.on = 1;
    tconfig.t32_enable = 0;
    tconfig.prescaler = 0;
    u_timerb_period_write(TIMER2, 8000);
    u_timerb_save_config(TIMER2, tconfig);
}
