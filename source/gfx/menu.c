//
// Created by yes on 5/20/26.
//

#include "menu.h"

#include <bdk.h>
#include <string.h>

#include "../util/error.h"
#include "../util/vector.h"
#include "../util/hid.h"
#include "gfx.h"
#include "../util/utils.h"

const char *sizeDefs[] = {
    "B  ",
    "KiB",
    "MiB",
    "GiB"
};

menuManager_t menuManager = {};

void _printEntry(entry_t entry, u32 maxLen) {

    switch (entry.type) {
    case ENTRY_END:
        drawError(newError(TE_ERROR_ACCESS_DENIED)); // TODO: Do better. Or stop wasting binary space.
        return;
    case ENTRY_SEPERATOR:
        gfxPutc('\n');
        return;
    case ENTRY_CAPTION:
    case ENTRY_HANDLER:
    case ENTRY_MENU:
    case ENTRY_BACK:
        if (entry.icon) {
            gfxPutc(entry.icon); gfxPutc(' ');
            maxLen -= 2;
        }

        u32 curX = 0, curY = 0;
        gfxConGetPos(&curX, &curY);

        if (entry.cursor)        SETCOLOR(INVERTCOLOR(entry.color), INVERTCOLOR(COLOR_BLACK));
        else if (entry.selected) SETCOLOR(entry.color, INVERTCOLOR(COLOR_BLACK));
        else                     SETCOLOR(            entry.color , COLOR_BLACK);

        if (!entry.showSize) {
            gfxPutsLimit(entry.caption, maxLen - 0);
            gfxPutc('\n');
        } else {
            gfxPutsLimit(entry.caption, maxLen - 8);
            gfxConSetPos(curX + (maxLen - 6) * gfx_con.fntsz, curY);
            gfx_printf("%d", entry.showSize);
            gfxPutsSmall(sizeDefs[entry.sizeIdx]);
        }
        SETCOLOR(COLOR_WHITE, COLOR_DEFAULT);
    // case ENTRY_DIR: // TODO:
    // case ENTRY_FILE: // TODO:
    }
}

void _renderDynamicMenu(menu_t m) {

}

void _renderStaticMenu(menu_t* m) {
    gfxConSetPos(0, 0);
    u32 lastDraw = get_tmr_us();

    if (!m->isOverlay) // TODO: This will not work with small entry refreshing
        gfxClearColor(COLORTOGREY(COLOR_DEFAULT));

    if (m->isPageCount) {
        SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
        gfx_printf("TegraExplorer 5.0.0");
        gfx_printf("                                                    ");
        char temp[40] = "";

        u16 itemsPerPage = (m->h              / gfx_con.fntsz);
        u16 totalPages   = (m->__static.count / itemsPerPage) + 1;
        u16 currentPage  = (m->offset         / itemsPerPage) + 1;

        s_printf(temp, " Page %d / %d | %d entries", currentPage, totalPages, m->__static.count); // Figure out actual pages. This might be tricky with variable font sizes though.
        gfxConSetPos(SCREEN_WIDTH - (strlen(temp) * gfx_con.fntsz), 0);
        gfx_printf(temp);
    }

    gfxConSetPos(m->x, m->y);
    gfxBoxGrey(m->x, m->y, m->x + m->w, m->y + m->h, COLORTOGREY(COLOR_DEFAULT));

    for (u16 i = 0; m->__static.entries[i].type != ENTRY_END; i++) {
        if (m->cursorIndex == i) m->__static.entries[i].cursor = true;
        else                     m->__static.entries[i].cursor = false;

        _printEntry(m->__static.entries[i], 0);
    }

    SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
    gfxConSetPos(0, SCREEN_HEIGHT - gfx_con.fntsz);
    // gfx_printf("Buttons active: %d", inputs->buttons);
    gfx_printf("Time taken for screen draw: %dus ", get_tmr_us() - lastDraw);
}

bool _canCursor(entryType_t type) {
    return (type == ENTRY_HANDLER || type == ENTRY_MENU || type == ENTRY_BACK || type == ENTRY_FILE || type == ENTRY_DIR);
}

void _handleInput(menu_t* m) {
    while (hidRead()) {
        if (RE_DETECTION(JOYPLUS)) {
            if (gfx_con.fntsz < 24) gfxConSetFontSize(gfx_con.fntsz += 1);
            break;
        }

        if (RE_DETECTION(JOYMINUS)) {
            if (gfx_con.fntsz > 8 ) gfxConSetFontSize(gfx_con.fntsz -= 1);
            break;
        }

        if (RE_DETECTION(JOYHOME)) {
            powerOff();
        }

        if (RE_DETECTION(JOYLUP)) {
            int prev = m->cursorIndex - 1;
            while (prev >= 0 && !_canCursor(m->__static.entries[prev].type))
                prev--;

            if (prev >= 0) m->cursorIndex = prev;
            break;
        }

        if (RE_DETECTION(JOYLDOWN)) {
            int next = m->cursorIndex + 1;
            while (next < m->__static.count && !_canCursor(m->__static.entries[next].type))
                next++;

            if (next < m->__static.count) m->cursorIndex = next;
            break;
        }

        if (RE_DETECTION(JOYA)) {
            entry_t entry = m->__static.entries[m->cursorIndex];
            switch (entry.type) {
            case ENTRY_HANDLER:
                entry.handler(NULL);
                break;
            case ENTRY_MENU:
                pushMenu(*(menu_t *)(entry.data));
                break;
            case ENTRY_BACK:
                popMenu();
                break;
            case ENTRY_FILE: // TODO: File menu
            case ENTRY_DIR:  // TODO: Folder menu

            default:
            }
            break;
        }

        if (RE_DETECTION(JOYY)) {
            entry_t entry = m->__static.entries[m->cursorIndex];
            switch (entry.type) {
            case ENTRY_FILE:
            case ENTRY_DIR:
                entry.selected = ~entry.selected;
            case ENTRY_HANDLER:
            case ENTRY_MENU:
            case ENTRY_BACK:
            default:

            }
            break;
        }
    }
}

void menuRenderTop() {
    menu_t* m = &menuManager.stack[menuManager.top];
    if (m->isDynamic) { // TODO: Do way better.
        drawError(newError(TE_ERROR_NOT_IMPL_YET)); // TODO: do better.
        powerOff();
    }

    _renderStaticMenu(m);

    vic_compose();
    vic_wait_idle();

    _handleInput(m);
}


void popMenu() {
    menu_t *m = &menuManager.stack[menuManager.top];
    if (m->isDynamic) {
        vecFree(m->__dynamic.data);
    }

    memset(&menuManager.stack[menuManager.top], 0, sizeof(menu_t));
    menuManager.top--;
}

void pushMenu(menu_t m) {
    if (menuManager.top++ >= MAX_STACK) drawError(newError(TE_ERROR_UNIMPLEMENTED)); // TODO: do better.
    menuManager.stack[menuManager.top] = m;
}
