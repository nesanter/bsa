#include "ulib/ulib.h"
#include "ulib/piezo.h"

void init_piezo() {
    u_oc_config config;

    config = u_oc_load_config(OC3);
    config.mode_select = 6;
    u_oc_save_config(OC3, config);
    u_pps_out_oc3(PPS_OUT4_RPB0);

    Pin pin = {PIN_GROUP_B, BITS(0)};
    pin_mode_set(pin, 0, 0);

    u_timerb_config tconfig = u_timerb_load_config(TIMER2);
    tconfig.on = 1;
    tconfig.t32_enable = 0;
    tconfig.prescaler = 0;
    u_timerb_period_write(TIMER2, 4000);
    u_timerb_save_config(TIMER2, tconfig);
}
