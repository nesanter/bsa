#include "ulib/ulib.h"
#include "ulib/js.h"

static int js_init = 0;

void init_js(void) {
    u_ana_config cfg = u_ana_load_config();
    cfg.on = 1;
    cfg.format = 5;
    cfg.conversion_trigger_source = 7;
    cfg.sample_auto_start = 1;
    cfg.sequences_per_interrupt = 1;
    cfg.auto_sample_time = 31;
    cfg.clock_select = 63;
    cfg.scan_inputs = 1;
//    cfg.result_buffer_mode = 1;

    //u_ana_set_mux(0, 3, 0, 0);
    u_ana_set_scan_select(BITS(2), 1);
    u_ana_set_scan_select(BITS(3), 1);

    Pin x_pin = {PIN_GROUP_B, BITS(0)}; // AN2
    Pin y_pin = {PIN_GROUP_B, BITS(1)}; // AN3
    pin_mode_set(x_pin, 1, 1);
    pin_mode_set(y_pin, 1, 1);

    u_ana_save_config(cfg);

    js_init = 1;
}

void deinit_js(void) {
    u_ana_config cfg = u_ana_load_config();
    cfg.on = 0;

    u_ana_save_config(cfg);
}

int get_js_init(void) {
    return js_init;
}

