#include <bdk.h>

#include "gfx/gfx.h"
#include "storage/emummc.h"
#include "util/config.h"
#include "util/hid.h"
#include "util/error.h"

#include "test_script.h"

#include "pikapython/pikascript-api/pikaScript.h"
#include "pikapython/pikascript-core/PikaVM.h"


extern void pivot_stack(u32 stack_top);

void _display_init() {
	vic_surface_t      vic_sfc;
	vic_sfc.src_buf  = NYX_FB2_ADDRESS;
	vic_sfc.dst_buf  = NYX_FB_ADDRESS;
	vic_sfc.width    = 1280;
	vic_sfc.height   = 720;
	vic_sfc.pix_fmt  = VIC_PIX_FORMAT_A8R8G8B8;
	vic_sfc.rotation = VIC_ROTATION_270;

	vic_init();
	vic_set_surface(&vic_sfc);

	vic_compose();
	vic_wait_idle();

	display_init_window_a_pitch_vic();
	gfx_init_ctxt((u32 *)NYX_FB2_ADDRESS, 1280,720,1280);
	gfx_con_init();

	display_backlight_pwm_init();
	display_backlight_brightness(80, 1000);

	gfx_clear_grey(0xb1);

	gfx_render_sdf();

	gfx_con.fntsz = 16;
	gfx_bake_atlas(gfx_con.fntsz);

	vic_compose();
	vic_wait_idle();
}

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

	TEConfig.errors    |= sd_mount() ? ERR_SD_BOOT_EN : false;
    TEConfig.errors    |= minerva_init(NULL) ? ERR_LIBSYS_LP0 : false;
	TEConfig.FSBuffSize = (TEConfig.minervaEnabled) ? 0x800000 : 0x10000;

	_display_init();

	emummc_load_cfg();

	// Ignore whether emummc is enabled.
	emu_cfg.enabled = !(emu_cfg.sector == true && !emu_cfg.path);
	TEConfig.emummcForceDisable = false;

	TEConfig.Pkg1ID = "Unknown";


	minerva_change_freq(FREQ_800);

	// PikaObj* pikaObj = pikaScriptInit();
	// pikaVM_runByteCode(pikaObj, __source_pikapython_pikascript_api_main_py_o);

	gfx_printf("\n\n\n");
	u32 before = get_tmr_us();
	gfx_test_putc('k');

	u32 after = get_tmr_us();
	gfx_printf("\nTook %dus", after - before);


	vic_compose();
	vic_wait_idle();
	hidWait();

	// drawError(newError(TE_ERROR_MEM_ALLOC_FAIL));

	power_set_state(POWER_OFF_RESET);

	while (true)
        bpmp_halt();

}