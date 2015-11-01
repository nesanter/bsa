#include "ulib/ulib.h"
#include "ulib/ldr.h"

void init_ldr() {
    u_ana_config cfg = u_ana_load_config();
    cfg.on = 1;
    cfg.format = 5;
    cfg.conversion_trigger_source = 7;
    cfg.sample_auto_start = 1;
    cfg.sequences_per_interrupt = 1;
    cfg.auto_sample_time = 31;
    cfg.clock_select = 63;

    u_ana_set_mux(0, 3, 0, 0);

    Pin ldr_pin = {PIN_GROUP_B, BITS(1)};
    pin_mode_set(ldr_pin, 1, 1);

    u_ana_save_config(cfg);
}

