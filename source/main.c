/*—————————————————————————————————————————————————————————————————————————————
 — Copyright (c) 2026 auggiethecat!                                           —
 —                                                                            —
 — All code was handwritten without the assistance of a Large Language Model. —
 —                                                                            —
 — This program is free software; you can redistribute it and/or modify it    —
 — under the terms and conditions of the GNU General Public License,          —
 — version 2, as published by the Free Software Foundation.                   —
 —                                                                            —
 — This program is distributed in the hope it will be useful, but WITHOUT     —
 — ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      —
 — FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for  —
 — more details.                                                              —
 —                                                                            —
 — You should have received a copy of the GNU General Public License          —
 — along with this program.  If not, see <http://www.gnu.org/licenses/>.      —
 —————————————————————————————————————————————————————————————————————————————*/

#include <memory_map.h>
#include <mem/heap.h>
#include <utils/types.h>
#include <soc/hw_init.h>
#include <display/di.h>
#include <soc/bpmp.h>
#include <soc/uart.h>

#include "configuration.h"
#include "gfx/gfx.h"
#include "gfx/menu.h"
#include "libs/lua/lua.h"
#include "libs/lua/lualib.h"
#include "menus/testMenu.h"
#include "util/error.h"
#include "util/hid.h"
#include "util/utils.h"

#if USE_VIC
#include <display/vic.h>
#endif

static void _displayInit() {
    display_init();

#if USE_VIC
    vic_surface_t      vic_sfc;
    vic_sfc.src_buf  = NYX_FB2_ADDRESS;
    vic_sfc.dst_buf  = NYX_FB_ADDRESS;
    vic_sfc.width    = SCREEN_WIDTH;
    vic_sfc.height   = SCREEN_HEIGHT;
    vic_sfc.pix_fmt  = VIC_PIX_FORMAT_X8R8G8B8;
    vic_sfc.rotation = VIC_ROTATION_270;

    vic_init();
    vic_set_surface(&vic_sfc);

    E_SHOW_ERROR_UNLESS(vic_compose(),   TE_ERROR_GENERAL_VIC_TIMEOUT);
    E_SHOW_ERROR_UNLESS(vic_wait_idle(), TE_ERROR_GENERAL_VIC_TIMEOUT);

    display_init_window_a_pitch_vic();
    gfxInitCtxt((u32 *)NYX_FB2_ADDRESS, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
#else
    display_init_window_a_pitch();
    // For now, this will just render sideways until, maybe if I want to do
    // software rotation configuration at compile time. I probably will do
    // that since I should test software rotation vs vic rotation speed anyway.
    gfxInitCtxt((u32 *)IPL_FB_ADDRESS, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

    gfxConInit();

    display_backlight_pwm_init();
    display_backlight_brightness(80, 1000);

#if SDF_FONT
    gfxBakeAtlas(8);
#endif

    gfxClearGrey(COLORTOGREY(COLOR_BG));
}


#define EXCEPTION_LR_ADDRESS     0x4003FF14
#define EXCEPTION_TYPE_ADDRESS   0x4003FF18
# define  RST 0x00545352
# define UNDF 0x46444e55
# define PABT 0x54424150
# define DABT 0x54424144
# define  WDT 0x00544457
#define EXCEPTION_ENABLE_ADDRESS 0x4003FF1C
# define EVP_            0x50564500
# define EXCEPTION_MAGIC EVP_

static void _showErrors() {
    if ((*(vu32*)EXCEPTION_ENABLE_ADDRESS & 0xFFFFFF00) != EXCEPTION_MAGIC)
        return;

    const u8 count = (*(u32*)EXCEPTION_ENABLE_ADDRESS & 0x000000FF);
    if (count >= 10) {
        powerOff(); // TODO: attempt an IRAM mode?
    }

    switch (*(vu32*)EXCEPTION_TYPE_ADDRESS) {
    case  RST: drawError(TE_EXCEPTION_RESET);          break;
    case UNDF: drawError(TE_EXCEPTION_UNDEFINED);      break;
    case PABT: drawError(TE_EXCEPTION_PREFETCH_ABORT); break;
    case DABT: drawError(TE_EXCEPTION_DATA_ABORT);     break;
    case  WDT: drawError(TE_EXCEPTION_WATCHDOG);       break;
      default: drawError(TE_EXCEPTION_UNKNOWN);        break;
    }
}

extern void pivotStack(u32 stack_top);

void iplMain() {
    hw_init();

    pivotStack(IPL_LOAD_ADDR);

    heap_init((void*)IPL_HEAP_START);

#ifdef DEBUG_UART_PORT
    uart_send(DEBUG_UART_PORT, (u8 *)"TegraExplorer!\n", 15);
    uart_wait_xfer(DEBUG_UART_PORT, UART_TX_IDLE);
#endif

    _displayInit();

    hidInit();

    _showErrors();

    pushTestMenu();

    lua_State* luastate = lua_newstate(NULL, NULL);
    luaL_openlibs(luastate);

    while (true)
        renderMenuTop();

    while (true)
        bpmp_halt();
}
