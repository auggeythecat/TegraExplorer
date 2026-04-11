#include <bdk.h>

#include "gfx/gfx.h"

extern void pivot_stack(u32 stack_top);

void ipl_main() {
    hw_init();

    jc_init_hw();

    pivot_stack(IPL_LOAD_ADDR);

    heap_init((void*)IPL_HEAP_START);

    display_init();

    bpmp_clk_rate_set(BPMP_CLK_LOWER_BOOST);

    sd_mount();

    minerva_init(NULL);

    u32 *fb = display_init_window_a_pitch();
    gfx_init_ctxt(fb, 720, 1280, 720);
    gfx_con_init();

    display_backlight_pwm_init();
    display_backlight_brightness(100, 1000);

    gfx_puts("\n\ntesting testing");

    while (true)
        bpmp_halt();

}