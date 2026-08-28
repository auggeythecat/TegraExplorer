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

#include "menu.h"

#include "../configuration.h"

#include <string.h>
#include <mem/heap.h>

#include "gfx.h"
#include "soc/timer.h"
#include "utils/sprintf.h"

#ifdef USE_VIC
#include <display/vic.h>
#endif

static menuManager_t menuManager = {};

void popMenu() {
    if (menuManager.top <= 1) return; // Please don't pop the main menu/

    const menu_t *m = &menuManager.stack[menuManager.top];
    if (m->entries) free(m->entries);

    memset(&menuManager.stack[menuManager.top], 0, sizeof(menu_t));
    menuManager.top--;
}

void pushMenu(const menu_t m) {
    if (menuManager.top++ >= MAX_MENU_STACK) return;
    menuManager.stack[menuManager.top] = m;
}

static void _printEntry(const menuEntry_t entry) {
    if (entry.type == ENTRY_END)
        __builtin_unreachable();

    if (entry.renderDirty) {
             if (entry.highlighted) SETCOLOR(INVERTCOLOR(entry.color), INVERTCOLOR(COLOR_BG));
        else if (entry.selected)    SETCOLOR(            entry.color , INVERTCOLOR(COLOR_BG));
        else                        SETCOLOR(            entry.color ,             COLOR_BG );

        // TODO: Putting filesize and icons.

        gfxPuts(entry.caption);
        gfxPutc('\n');
    }
}

static void _printHeader(const menu_t* m) {
    if (!m->headerDirty)
        return;

    gfxConSetPos(0,0);
    gfxConSetCol(RGBTOCOLOR(0xFF, 0x8E, 0x07), FILLBG, RGBTOCOLOR(0xEF, 0xDC, 0xD3));

    // TODO: Check if this makes a major performance difference.
    // If it does, the best path forward might be to make this render with a white background color,
    // (makes character printing faster because no alpha mult first of all)
    // but also, it *might* remove the need to redraw the whole box.
    // I guess you would also want to add some padding on the back
    // (ie, add multiple spaces before the text, since that would just throw the bg color into the fb)
    gfxBoxGrey(0, 0, SCREEN_WIDTH, gfxCon.fntsz, 0xFF);

    gfxPrintF("TegraExplorer %d.%d.%d", TE_VER_MJ, TE_VER_MN, TE_VER_HF);

    const u16 itemsPerPage = (m->h           / gfxCon.fntsz)    ;
    const u16 totalPages   = (m->count       / itemsPerPage) + 1;
    const u16 currentPage  = (m->cursorIndex / itemsPerPage) + 1;

    char temp[40];
    s_printf(temp, " Page %d / %d | %d entries", currentPage, totalPages, m->count);
    gfxConSetPos(SCREEN_WIDTH - (strlen(temp) * gfxCon.fntsz), 0);
    gfx_printf(temp);
}

static void _printFooter(const u32 lastDraw) {
    gfxConSetPos(0, SCREEN_HEIGHT - gfx_con.fntsz);
    gfx_printf("Time taken for screen draw: %dus ", get_tmr_us() - lastDraw);
}

static void _handleInput(const menu_t* m) {
    const menuEntry_t entry = m->entries[m->cursorIndex];

    switch (entry.type) {
    case ENTRY_END:
    case ENTRY_SEPERATOR:
    case ENTRY_CAPTION:
    case ENTRY_HANDLER:
    case ENTRY_HANDLER_EX:
    case ENTRY_MENU:
    case ENTRY_DIRECTORY:
    case ENTRY_FILE:
    case ENTRY_BACK:
    default:
        break;
    }
}

void renderMenuTop() {
    const u32 before = get_tmr_us();
    const menu_t* m = &menuManager.stack[menuManager.top];

    if (m->renderDirty)
        gfxClearGrey(0xF8);

    if (m->printHeader)
        _printHeader(m);

    gfxConSetPos(m->x, m->y);
    for (u32 i = 0; i < m->count || m->entries[i].type != ENTRY_END; i++)
        _printEntry(m->entries[i]);

    if (m->printFooter)
        _printFooter(before);

#ifdef USE_VIC
    vic_compose();
    vic_wait_idle();
#endif

    _handleInput(m);
}