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

#include <string.h>
#include <mem/heap.h>
#include <soc/timer.h>
#include <utils/sprintf.h>

#include "gfx.h"
#include "../util/hid.h"
#include "../util/utils.h"

#if USE_VIC
#include <display/vic.h>
#endif

static menuManager_t menuManager = {};

#define     MENU_TOP     ( menuManager.stack[menuManager.top    ])
#define GET_MENU_TOP     (&menuManager.stack[menuManager.top    ])
#define GET_MENU(n)      (&menuManager.stack[                  n])
#define GET_MENU_BACK(n) (&menuManager.stack[menuManager.top - n])

void popMenu() {
    if (menuManager.top <= 1) return; // Please don't pop the main menu.

    menu_t *m = GET_MENU_TOP;
    for (u32 i = 0; i > m->count; i++) {
        if (!m->entries[i].captionStatic)
            free(m->entries[i].caption);
    }

    if (m->entries) free(m->entries);

    memset(m, 0, sizeof(menu_t));
    menuManager.top--;

    MENU_TOP.renderDirty = true;
}

void pushMenu(const menu_t m) {
    if (menuManager.top++ >= MAX_MENU_STACK) return;
    MENU_TOP = m;
}

static void _printEntry(menuEntry_t entry, const u32 maxLenChars) {
    if (!menuManager.fullRedraw && !MENU_TOP.renderDirty && !entry.renderDirty)
        return;

    const u32 fgColor = entry.highlighted ? INVERTCOLOR(entry.color) : entry.color;
    const u32 bgColor = entry.selected    ? INVERTCOLOR(COLOR_BG   ) : COLOR_BG;
    SETCOLOR(fgColor, bgColor);

    gfxPutSLimit(entry.caption, maxLenChars);
    entry.renderDirty = false;
}

static void _renderMenus() {
    menu_t* m;
    u32 n = 0;
    do {
        m = GET_MENU_BACK(n);

        if (n > menuManager.top) {
            // Panic.
            powerOff();
        }

        if (m->fullHeight) {
            m->y = gfxCon.fntsz;
            m->h = SCREEN_HEIGHT - (2 * gfxCon.fntsz);
        }

        if (m->renderDirty)
            gfxBoxGrey(m->x, m->y, m->w, m->h, COLORTOGREY(COLOR_DEFAULT));

        gfxConSetPos(m->x, m->y);

        const u32 maxLengthCharacters = m->w / gfxCon.fntsz;

        m->entries[m->cursorIndex].highlighted = true;
        for (u32 i = 0; i < m->count; i++) {
            if (m->entries[i].type == ENTRY_END) break;
            _printEntry(m->entries[i], maxLengthCharacters);
            gfxPutC('\n');
        }

        m->renderDirty = false;
        n++;
    } while (m->isOverlay);
}

static void _printHeader() {
    menu_t *m = GET_MENU_TOP;

    // TODO: Check if this makes a major performance difference.
    // If it does, the best path forward might be to make this render with a white background color,
    // (makes character printing faster because no alpha mult first of all)
    // but also, it *might* remove the need to redraw the whole box.
    // I guess you would also want to add some padding on the back
    // (ie, add multiple spaces before the text, since that would just throw the bg color into the fb)
    gfxBoxGrey(0, 0, SCREEN_WIDTH, gfxCon.fntsz, COLORTOGREY(COLOR_BLACK));
    gfxConSetCol(COLOR_DEFAULT, NOFILLBG, COLOR_WHITEST);
    gfxConSetPos(0, 0);
    gfxPrintF("TegraExplorer version %d.%d.%d", TE_VER_MJ, TE_VER_MN, TE_VER_HF);

    const u16 itemsPerPage = (m->h           / gfxCon.fntsz)    ;
    const u16 totalPages   = (m->count       / itemsPerPage) + 1;
    const u16 currentPage  = (m->cursorIndex / itemsPerPage) + 1;

    char temp[40];
    s_printf(temp, " Page %d / %d | %d entries", currentPage, totalPages, m->count);
    gfxConSetPos(SCREEN_WIDTH - (strlen(temp) * gfxCon.fntsz), 0);
    gfxPrintF(temp);
}

/*
 * TODO: Add a fill parameter to entries and make header/footer entries?
 * Though, how will this work with pagination? Maybe have a certain
 * amount that can be scrolled? This would also be nice for the file
 * browser, which would let you have file/folder/back options at the top
 * always. But how would vertical scrolling work in that case? Go up to
 * the entries that can't move, and then it will scroll the ones below
 * it? This will increase binary size...
 *
 * But this won't work with menus that aren't full-width. Should it be
 * another menu overlayed on top of the others? No, that introduces more
 * problems, and removes the benefit of the previous suggestion.
 */

static void _printFooter() {
    gfxBoxGrey(0, SCREEN_HEIGHT - gfxCon.fntsz, SCREEN_WIDTH, SCREEN_HEIGHT, COLORTOGREY(COLOR_BLACK));
    gfxConSetCol(COLOR_DEFAULT, NOFILLBG, COLOR_WHITEST);
    gfxConSetPos(0, SCREEN_HEIGHT - gfxCon.fntsz);
    gfxPrintF("Time taken for screen draw: %dus ", get_tmr_us() - menuManager.lastDraw);
}

static void _handleInput() {
    menu_t *m = GET_MENU_TOP;
    menuEntry_t* entry = &m->entries[m->cursorIndex];

    while (hidRead()) {
        if (RE_INPUT_DETECTION(JOYPLUS)) {
            if (gfx_con.fntsz < 24) gfxConSetFontSize(gfx_con.fntsz += 1);
            menuManager.fullRedraw = true;
            break;
        }

        if (RE_INPUT_DETECTION(JOYMINUS)) {
            if (gfx_con.fntsz > 8) gfxConSetFontSize(gfx_con.fntsz -= 1);
            menuManager.fullRedraw = true;
            break;
        }

        if (RE_INPUT_DETECTION(JOYHOME)) {
            powerOff();
            break;
        }

        if (RE_INPUT_DETECTION(JOYA)) {
#if UB_FUNCTION_POINTER_HACK
            entry->handler(entry->data);
#else
            entry->handleWithArgs ?
            entry->__handler.with_arg(entry->data) :
            entry->__handler.no_arg  (           ) ;
#endif
            break;
        }

        if (RE_INPUT_DETECTION(JOYB)) {
            popMenu();
            break;
        }

        if (RE_INPUT_DETECTION(JOYX)) {
            if (entry->selectable) entry->selected = ~entry->selected;
            entry->renderDirty = true;
            break;
        }

        if (RE_INPUT_DETECTION(JOYLDOWN)) {
            while (m->entries[m->cursorIndex+1].type != ENTRY_END) {
                m->cursorIndex++;
                menuEntry_t* entry2 = &m->entries[m->cursorIndex];
                if (entry2->skip)
                    continue;

                entry->renderDirty  = true;
                entry2->renderDirty = true;
                entry->highlighted  = false;
                entry2->highlighted = true;
                break;
            }
            break;
        }

        if (RE_INPUT_DETECTION(JOYLUP)) {
            while (m->cursorIndex != 0) {
                m->cursorIndex--;
                menuEntry_t *entry2 = &m->entries[m->cursorIndex];
                if (entry2->skip)
                    continue;

                entry->renderDirty  = true;
                entry2->renderDirty = true;
                entry->highlighted  = false;
                entry2->highlighted = true;
                break;
            }
            break;
        }
    }
}

void renderMenuTop() {
    menuManager.lastDraw = get_tmr_us();

    if (menuManager.fullRedraw)
        gfxClearGrey(COLORTOGREY(COLOR_DEFAULT));

    _renderMenus();

    _printFooter();
    _printHeader();

#if USE_VIC
    vic_compose();
#endif

    _handleInput();
}