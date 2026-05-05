#include <bdk.h>

#include "gfx/gfx.h"
#include "storage/emummc.h"
#include "util/config.h"
#include "util/hid.h"


extern void pivot_stack(u32 stack_top);

void ipl_main() {
    hw_init();

    hidInit();

    pivot_stack(IPL_LOAD_ADDR);

    heap_init((void*)IPL_HEAP_START);

#ifdef DEBUG_UART_PORT
	uart_send(DEBUG_UART_PORT, (u8 *)"te: Hello!\r\n", 12);
	uart_wait_idle(DEBUG_UART_PORT, UART_TX_IDLE);
#endif

    display_init();

    bpmp_clk_rate_set(BPMP_CLK_LOWER_BOOST);

	TEConfig.errors    |= sd_mount() ? ERR_SD_BOOT_EN : false;
    TEConfig.errors    |= minerva_init(NULL) ? ERR_LIBSYS_LP0 : false;
	TEConfig.FSBuffSize = (TEConfig.minervaEnabled) ? 0x800000 : 0x10000;


    u32 *fb = display_init_window_a_pitch();
    gfx_init_ctxt(fb, 1280, 720, 720);
    gfx_con_init();

    display_backlight_pwm_init();
    display_backlight_brightness(100, 1000);

	emummc_load_cfg();

	// Ignore whether emummc is enabled.
	emu_cfg.enabled = !(emu_cfg.sector == true && !emu_cfg.path);
	TEConfig.emummcForceDisable = false;

	TEConfig.Pkg1ID = "Unknown";


	minerva_change_freq(FREQ_800);

	btn_wait();
	power_set_state(POWER_OFF_RESET);


	while (true)
        bpmp_halt();

}