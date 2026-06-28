#include <bdk.h>

#include "gfx/gfx.h"
#include "storage/emummc.h"
#include "util/config.h"
#include "util/hid.h"
#include "util/error.h"

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

	gfxClearGrey(0xb1);

	vic_compose();
	vic_wait_idle();
}

void ipl_main() { // Mus
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

	// key_storage_t keys = {};
	// derive_relevant_keys(&keys);

	entry_t testEntries2[] = {
		{ ENTRY_SEPERATOR, COLOR_TRANSPARENT,0,              NULL, NULL    },
		{ ENTRY_CAPTION,   COLOR_WHITE,      "Menu name 2!", NULL, NULL    },
		{ ENTRY_SEPERATOR, COLOR_TRANSPARENT,0,              NULL, NULL    },
		{ ENTRY_BACK     , COLOR_GREEN,"Go back to where you came from!",              NULL, NULL    },
		{ ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
		{ ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
		{ ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
		{ ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_END,       COLOR_TRANSPARENT,0,              NULL, NULL    },

	};

	menu_t testMenu2 = {
		.title = "My Test 2",
		.x     = 0,    .y = 0,
		.w     = 1280, .h = 720,
		.idx   = 0,    .offset = 0,

		.isOverlay   = 0,
		.isDynamic   = 0,
		.isPageCount = 1,
		.cursorIndex = 3,

		.__static.entries = testEntries2,
		.__static.count   = ARRAY_SIZE(testEntries2)
	};


    entry_t testEntries[] = {
	    { ENTRY_SEPERATOR, COLOR_TRANSPARENT,0,              NULL, NULL    },
	    { ENTRY_CAPTION,   COLOR_WHITE,      "Menu name!!!", NULL, NULL    },
	    { ENTRY_SEPERATOR, COLOR_TRANSPARENT,0,              NULL, NULL    },
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_MENU   ,   COLOR_YELLOW,     "Menu test",    &testMenu2, NULL},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_END,       COLOR_TRANSPARENT,0,              NULL, NULL    },
    };


	menu_t testMenu = {
		.title = "My Test",
		.x     = 0,    .y = 0,
		.w     = 1280, .h = 720,
		.idx   = 0,    .offset = 0,


		.isOverlay   = 0,
		.isDynamic   = 0,
		.isPageCount = 1,
		.cursorIndex = 3,

		.__static.entries = testEntries,
		.__static.count   = ARRAY_SIZE(testEntries)
	};

	pushMenu(testMenu);

	while (true)
		menuRenderTop();

	while (true)
        bpmp_halt();
}