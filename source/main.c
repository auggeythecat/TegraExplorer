#include <bdk.h>

#include "gfx/gfx.h"
#include "storage/emummc.h"
#include "util/config.h"
#include "util/hid.h"
#include "util/error.h"
#include "storage/safeStorage.h"
#include "menus/mainMenu.h"

#include "test_script.h"
#include "gfx/menu.h"
#include "keys/keys.h"

#include "pikapython/pikascript-api/pikaScript.h"
#include "pikapython/pikascript-core/PikaVM.h"
#include "util/utils.h"


extern void pivotStack(u32 stack_top);

void _displayInit() {
	vic_surface_t      vic_sfc;
	vic_sfc.src_buf  = NYX_FB2_ADDRESS;
	vic_sfc.dst_buf  = NYX_FB_ADDRESS;
	vic_sfc.width    = SCREEN_WIDTH;
	vic_sfc.height   = SCREEN_HEIGHT;
	vic_sfc.pix_fmt  = VIC_PIX_FORMAT_A8R8G8B8;
	vic_sfc.rotation = VIC_ROTATION_270;

	display_init();

	vic_init();
	vic_set_surface(&vic_sfc);

	vic_compose();
	vic_wait_idle();

	display_init_window_a_pitch_vic();
	gfxInitCtxt((u32 *)NYX_FB2_ADDRESS, 1280,720,1280);
	gfxConInit();

	display_backlight_pwm_init();
	display_backlight_brightness(80, 1000);

	gfxBakeAtlas(8);
}

void _testsdmmc() {
    u8* buf[256];
    safeSdmmcStorageRead(&sd_storage, 0, 1, buf);
    // 0, success, 1, read failure, -1, irq fired.
    gfxHexDump(16, buf, 256);
}

void ipl_main() {
    hw_init();

    pivotStack(IPL_LOAD_ADDR);

    heap_init((void*)IPL_HEAP_START);

#ifdef DEBUG_UART_PORT
	uart_send(DEBUG_UART_PORT, (u8 *)"te: Hello!\r\n", 12);
	uart_wait_idle(DEBUG_UART_PORT, UART_TX_IDLE);
#endif

    bpmp_clk_rate_set(BPMP_CLK_LOWER_BOOST);

	TEConfig.errors    |= sd_mount()              ? ERR_SD_BOOT_EN : false;
    TEConfig.errors    |= minerva_init(NULL)      ? ERR_LIBSYS_LP0 : false;
	TEConfig.FSBuffSize = TEConfig.minervaEnabled ? 0x800000       : 0x10000;

	_displayInit();

    hidInit();

	emummcLoadCFG();

	emuCFG.enabled = !(emuCFG.sector == true && !emuCFG.path);
	TEConfig.emummcForceDisable = false;

	TEConfig.Pkg1ID = "Unknown";

	minerva_change_freq(FREQ_800);

	// PikaObj* pikaObj = pikaScriptInit();

	// keyStorage_t keys = {};
	// deriveRelevantKeys(&keys);

    pushMainMenu();

	while (true)
		menuRenderTop();

	while (true)
        bpmp_halt();
}