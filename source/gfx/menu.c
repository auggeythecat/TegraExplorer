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
#include <soc/timer.h>
#include <utils/sprintf.h>

#include "gfx.h"
#include "../util/hid.h"
#include "../util/math.h"
#include "../util/utils.h"

#ifdef USE_VIC
#include <display/vic.h>
#endif

static menuManager_t menuManager = {};

void popMenu() {
    if (menuManager.top <= 1) return; // Please don't pop the main menu.

    const menu_t *m = &menuManager.stack[menuManager.top];
    if (m->entries) free(m->entries);

    memset(&menuManager.stack[menuManager.top], 0, sizeof(menu_t));
    menuManager.top--;

    menuManager.stack[menuManager.top].renderDirty = true;
}

void pushMenu(const menu_t m) {
    if (menuManager.top++ >= MAX_MENU_STACK) return;
    menuManager.stack[menuManager.top] = m;
}

// TODO: Decide on how I should format this.
static char* fileSize[4] = {
    "B",   // "B", or "  B", or " B", or something else
    "KiB", // "K", or "KiB", or "KB", or something else
    "MiB", // "M", or "MiB", or "MB", or something else
    "GiB"  // "G", or "GiB", or "GB", or something else
};

static void _printEntry(menuEntry_t entry, u32 maxLen) {
    if (entry.renderDirty) {
             if (entry.highlighted) SETCOLOR(INVERTCOLOR(entry.color), INVERTCOLOR(COLOR_BG));
        else if (entry.selected)    SETCOLOR(            entry.color , INVERTCOLOR(COLOR_BG));
        else                        SETCOLOR(            entry.color ,             COLOR_BG );

        const u32 fontSize = gfxCon.fntsz;

        if (entry.showIcon) {
            // TODO: Is there an actual way to concat characters better then this?
            gfxPutC(entry.icon);
            gfxPutC('|');

            maxLen -= 2*fontSize;
        }

        if (entry.showSize) {
            const u32 numLength = log10Approximation(entry.fileSize);
            const u32 sizeLength = strlen(fileSize[entry.fileSizeIndex]);

            maxLen -= (numLength*fontSize) + (sizeLength*fontSize);

            gfxPutSLimit(entry.caption, maxLen);
            gfxPrintF("%d%s", entry.fileSize, fileSize[entry.fileSizeIndex]);
        } else
            gfxPutSLimit(entry.caption, maxLen);

        entry.renderDirty = false;
    }
}

static void _printHeader(menu_t* m) {
    gfxConSetPos(0,0);
    gfxConSetCol(RGBTOCOLOR(0xFF, 0x8E, 0x07), FILLBG, RGBTOCOLOR(0xEF, 0xDC, 0xD3));

    // TODO: Check if this makes a major performance difference.
    // If it does, the best path forward might be to make this render with a white background color,
    // (makes character printing faster because no alpha mult first of all)
    // but also, it *might* remove the need to redraw the whole box.
    // I guess you would also want to add some padding on the back
    // (ie, add multiple spaces before the text, since that would just throw the bg color into the fb)
    gfxBoxGrey(0, 0, SCREEN_WIDTH, gfxCon.fntsz, 0xFF);

    gfxPrintF("TEGRAEXPLORER %d.%d.%d", TE_VER_MJ, TE_VER_MN, TE_VER_HF);

    const u16 itemsPerPage = (m->h           / gfxCon.fntsz)    ;
    const u16 totalPages   = (m->count       / itemsPerPage) + 1;
    const u16 currentPage  = (m->cursorIndex / itemsPerPage) + 1;

    char temp[40];
    s_printf(temp, " PAGE %d / %d | %d ENTRIES", currentPage, totalPages, m->count);
    gfxConSetPos(SCREEN_WIDTH - (strlen(temp) * gfxCon.fntsz), 0);
    gfxPrintF(temp);
}

static void _printFooter(menu_t* m) {
    gfxBoxGrey(0, SCREEN_HEIGHT - gfxCon.fntsz, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFF);
    gfxConSetCol(RGBTOCOLOR(0xFF, 0x8E, 0x07), FILLBG, RGBTOCOLOR(0xEF, 0xDC, 0xD3));
    gfxConSetPos(0, SCREEN_HEIGHT - gfxCon.fntsz);
    gfxPrintF("TIME TAKEN FOR SCREEN DRAW: %dUS ", get_tmr_us() - m->lastDraw);
}

static void _handleInput(menu_t* m) {
    menuEntry_t entry = m->entries[m->cursorIndex];

    while (hidRead()) {
        if (RE_INPUT_DETECTION(JOYPLUS)) {
            if (gfx_con.fntsz < 24) gfxConSetFontSize(gfx_con.fntsz += 1);
            m->renderDirty = true;
            break;
        }

        if (RE_INPUT_DETECTION(JOYMINUS)) {
            if (gfx_con.fntsz > 8) gfxConSetFontSize(gfx_con.fntsz -= 1);
            m->renderDirty = true;
            break;
        }

        if (RE_INPUT_DETECTION(JOYHOME)) {
            powerOff();
            break;
        }

        if (RE_INPUT_DETECTION(JOYA)) {
            entry.handler();
            break;
        }

        if (RE_INPUT_DETECTION(JOYX)) {
            if (entry.selectable) entry.selected = ~entry.selected;
            entry.renderDirty = true;
            break;
        }

        if (RE_INPUT_DETECTION(JOYLDOWN)) {
            while (m->entries[m->cursorIndex+1].type != ENTRY_END) {
                m->cursorIndex++;
                menuEntry_t entry2 = m->entries[m->cursorIndex];
                if (entry2.skip)
                    continue;

                entry.renderDirty  = true;
                entry2.renderDirty = true;
                entry.highlighted  = false;
                entry2.highlighted = true;
                break;
            }
            break;
        }

        if (RE_INPUT_DETECTION(JOYLUP)) {
            while (m->cursorIndex != 0) {
                m->cursorIndex--;
                menuEntry_t entry2 = m->entries[m->cursorIndex];
                if (entry2.skip)
                    continue;

                entry.renderDirty  = true;
                entry2.renderDirty = true;
                entry.highlighted  = false;
                entry2.highlighted = true;
                break;
            }
            break;
        }
    }
}

void renderMenuTop() {
    menu_t* m = &menuManager.stack[menuManager.top];
    m->lastDraw = get_tmr_us();

    if (m->renderDirty) {
        gfxClearGrey(0x4F);
        m->renderDirty = false;
    }

    if (m->printHeader || m->renderDirty) {
        _printHeader(m);
        m->headerDirty = false;
    }

    gfxConSetPos(m->x, m->y);

    const u32 maxLength = m->w;

    m->entries[m->cursorIndex].highlighted = true;
    for (u32 i = 0; i < m->count; i++) {
        if (m->entries[i].type == ENTRY_END) break;
        _printEntry(m->entries[i], maxLength);
        gfxPutC('\n');
    }

    if (m->printFooter || m->renderDirty) {
        _printFooter(m);
        m->footerDirty = false;
    }

#ifdef USE_VIC
    vic_compose();
    vic_wait_idle();
#endif

    // Until I find a better solution, this is how I will ensure
    // the footer will always be updated every render. I don't
    // really like this solution, but I can't think of much better.
    // Maybe I should just remove render dirtying for the footer
    // entirely? Maybe add another bool in the bitfield to do this
    // in case of other footers that don't require constant updating?
    if (m->printFooter) m->footerDirty = true;

    _handleInput(m);
}