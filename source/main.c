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

#include "gfx/gfx.h"
#include "configuration.h"
#include "gfx/menu.h"
#include "menus/testMenu.h"
#include "util/hid.h"

#ifdef USE_VIC
#include <display/vic.h>
#endif

static void _displayInit() {
    display_init();

#ifdef USE_VIC
    vic_surface_t      vic_sfc;
    vic_sfc.src_buf  = NYX_FB2_ADDRESS;
    vic_sfc.dst_buf  = NYX_FB_ADDRESS;
    vic_sfc.width    = SCREEN_WIDTH;
    vic_sfc.height   = SCREEN_HEIGHT;
    vic_sfc.pix_fmt  = VIC_PIX_FORMAT_X8R8G8B8;
    vic_sfc.rotation = VIC_ROTATION_270;

    vic_init();
    vic_set_surface(&vic_sfc);

    vic_compose();
    vic_wait_idle();

    display_init_window_a_pitch_vic();
#else
    display_init_window_a_pitch();
#endif

    gfxInitCtxt((u32 *)NYX_FB2_ADDRESS, 1280,720,1280);
    gfxConInit();

    display_backlight_pwm_init();
    display_backlight_brightness(80, 1000);

    gfxBakeAtlas(8);
}

extern void pivot_stack(u32 stack_top);
void ipl_main() __attribute__((alias("iplMain")));
void iplMain() {
    hw_init();

    pivot_stack(IPL_LOAD_ADDR);

    heap_init((void*)IPL_HEAP_START);

    _displayInit();

    pushTestMenu();

    hidInit();

    while (true)
        renderMenuTop();

    while (true)
        bpmp_halt();
}