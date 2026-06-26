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


extern void pivot_stack(u32 stack_top);

void _display_init() {
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
	gfx_init_ctxt((u32 *)NYX_FB2_ADDRESS, 1280,720,1280);
	gfx_con_init();

	display_backlight_pwm_init();
	display_backlight_brightness(80, 1000);

	gfx_clear_grey(0xb1);

	vic_compose();
	vic_wait_idle();
}

void ipl_main() {
    hw_init();

    pivot_stack(IPL_LOAD_ADDR);

    heap_init((void*)IPL_HEAP_START);

#ifdef DEBUG_UART_PORT
	uart_send(DEBUG_UART_PORT, (u8 *)"te: Hello!\r\n", 12);
	uart_wait_idle(DEBUG_UART_PORT, UART_TX_IDLE);
#endif

    bpmp_clk_rate_set(BPMP_CLK_LOWER_BOOST);

	TEConfig.errors    |= sd_mount()              ? ERR_SD_BOOT_EN : false;
    TEConfig.errors    |= minerva_init(NULL)      ? ERR_LIBSYS_LP0 : false;
	TEConfig.FSBuffSize = TEConfig.minervaEnabled ? 0x800000       : 0x10000;

	_display_init();

    hidInit();

	emummc_load_cfg();

	emu_cfg.enabled = !(emu_cfg.sector == true && !emu_cfg.path);
	TEConfig.emummcForceDisable = false;

	TEConfig.Pkg1ID = "Unknown";

	minerva_change_freq(FREQ_800);

	// PikaObj* pikaObj = pikaScriptInit();

	// key_storage_t keys = {};
	// derive_relevant_keys(&keys);

	entry_t test_entries_2[] = {
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

	menu_t test_menu_2 = {
		.title = "My Test 2",
		.x     = 0,    .y = 0,
		.w     = 1280, .h = 720,
		.idx   = 0,    .offset = 0,

		.is_overlay  = 0,
		.is_dynamic  = 0,
		.page_count  = 1,
		.cursorIndex = 3,
		.reserved    = 0,

		.__static.entries = test_entries_2,
		.__static.count   = ARRAY_SIZE(test_entries_2)
	};


    entry_t test_entries[] = {
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
	    { ENTRY_MENU   ,   COLOR_YELLOW,     "Menu test",    &test_menu_2, NULL},
	    { ENTRY_HANDLER,   COLOR_BLUE,       "Power off",    NULL, powerOff},
	    { ENTRY_END,       COLOR_TRANSPARENT,0,              NULL, NULL    },
    };


	menu_t test_menu = {
		.title = "My Test",
		.x     = 0,    .y = 0,
		.w     = 1280, .h = 720,
		.idx   = 0,  .offset = 0,


		.is_overlay  = 0,
		.is_dynamic  = 0,
		.page_count  = 1,
		.cursorIndex = 3,
		.reserved    = 0,

		.__static.entries = test_entries,
		.__static.count   = ARRAY_SIZE(test_entries)
	};

	push_menu(test_menu);

	while (true)
		menu_render_top();

	power_set_state(POWER_OFF_RESET);

	while (true)
        bpmp_halt();
}