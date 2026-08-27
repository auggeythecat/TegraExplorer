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

static void _printHeader(const menu_t* m) {
    if (!m->headerDirty)
        return;

    gfxConSetPos(0,0);
    gfxConSetCol(RGBTOCOLOR(0xFF, 0x8E, 0x07), FILLBG, RGBTOCOLOR(0xEF, 0xDC, 0xD3));

    if (m->renderDirty) {
        // TODO: fill the bg of the header
    }

    gfxPrintF("TegraExplorer %d.%d.%d", TE_VER_MJ, TE_VER_MN, TE_VER_HF);

    // TODO: The right side.
}

static void _printFooter(const menu_t* m) {

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
    const menu_t* m = &menuManager.stack[menuManager.top];

    if (m->printHeader)
        _printHeader(m);

    for (u32 i = 0; i < m->count; i++) {
        const menuEntry_t entry = m->entries[i];
        switch (entry.type) {
        case ENTRY_END:
        case ENTRY_SEPERATOR:
            if (entry.renderDirty)
                gfxPutc('\n');
            break;

        case ENTRY_CAPTION:
        case ENTRY_HANDLER:
        case ENTRY_HANDLER_EX:
        case ENTRY_MENU:
        case ENTRY_DIRECTORY:
        case ENTRY_FILE:
        case ENTRY_BACK:
            if (entry.renderDirty) {
                gfxCPuts(entry.color, entry.caption);
                gfxPutc('\n');
            }
            break;

        default:
            break;
        }
    }

    _printFooter(m);

#ifdef USE_VIC
    vic_compose();
    vic_wait_idle();
#endif

    _handleInput(m);
}