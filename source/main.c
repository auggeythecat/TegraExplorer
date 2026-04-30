#include <bdk.h>

#include "gfx/gfx.h"
#include "storage/emummc.h"
#include "util/config.h"

extern void pivot_stack(u32 stack_top);

void ipl_main() {
    hw_init();

    jc_init_hw();

    pivot_stack(IPL_LOAD_ADDR);

    heap_init((void*)IPL_HEAP_START);

#ifdef DEBUG_UART_PORT
	uart_send(DEBUG_UART_PORT, (u8 *)"te: Hello!\r\n", 12);
	uart_wait_idle(DEBUG_UART_PORT, UART_TX_IDLE);
#endif

    display_init();

    bpmp_clk_rate_set(BPMP_CLK_LOWER_BOOST);

	TEConfig.errors |= sd_mount() ? ERR_SD_BOOT_EN : 0;

    TEConfig.errors |= minerva_init(NULL) ? ERR_LIBSYS_LP0 : 0;

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

    gfx_puts("\n\n");

	for (int i = 1; i <= 250; i++) {
		gfx_puts("testing ");
	}

	minerva_change_freq(FREQ_800);

	btn_wait();
	power_set_state(POWER_OFF_RESET);


	while (true)
        bpmp_halt();

}