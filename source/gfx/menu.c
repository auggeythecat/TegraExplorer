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

menu_manager_t menu_manager = {};

void _printEntry(entry_t entry, u32 maxLen) {

    switch (entry.type) {
    case ENTRY_END:
        drawError(newError(TE_ERROR_ACCESS_DENIED)); // TODO: Do better. Or stop wasting binary space.
        return;
    case ENTRY_SEPERATOR:
        gfx_putc('\n');
        return;
    case ENTRY_CAPTION:
    case ENTRY_HANDLER:
    case ENTRY_MENU:
    case ENTRY_BACK:
        if (entry.icon) {
            gfx_putc(entry.icon); gfx_putc(' ');
            maxLen -= 2;
        }

        u32 curX = 0, curY = 0;
        gfx_con_getpos(&curX, &curY);

        if (entry.cursor)        SETCOLOR(INVERTCOLOR(entry.color), INVERTCOLOR(COLOR_BLACK));
        else if (entry.selected) SETCOLOR(entry.color, INVERTCOLOR(COLOR_BLACK));
        else                     SETCOLOR(            entry.color , COLOR_BLACK);

        if (!entry.showSize) {
            gfx_puts_limit(entry.caption, maxLen - 0);
            gfx_putc('\n');
        } else {
            gfx_puts_limit(entry.caption, maxLen - 8);
            gfx_con_setpos(curX + (maxLen - 6) * gfx_con.fntsz, curY);
            gfx_printf("%d", entry.showSize);
            gfx_puts_small(sizeDefs[entry.sizeIdx]);
        }
        SETCOLOR(COLOR_WHITE, COLOR_DEFAULT);
    // case ENTRY_DIR: // TODO:
    // case ENTRY_FILE: // TODO:
    }
}

void _render_dynamic_menu(menu_t m) {

}

void _render_static_menu(menu_t* m) {
    gfx_con_setpos(0, 0);
    u32 lastDraw = get_tmr_us();

    if (!m->is_overlay) // TODO: This will not work with small entry refreshing
        gfx_clear_color(COLORTOGREY(COLOR_DEFAULT));

    if (m->page_count) {
        SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
        gfx_printf("TegraExplorer 5.0.0");
        gfx_printf("                                                    ");
        char temp[40] = "";

        u16 items_per_page = (m->h              / gfx_con.fntsz);
        u16 total_pages    = (m->__static.count / items_per_page) + 1;
        u16 current_page   = (m->offset         / items_per_page) + 1;

        s_printf(temp, " Page %d / %d | %d entries", current_page, total_pages, m->__static.count); // Figure out actual pages. This might be tricky with variable font sizes though.
        gfx_con_setpos(SCREEN_WIDTH - (strlen(temp) * gfx_con.fntsz), 0);
        gfx_printf(temp);
    }

    gfx_con_setpos(m->x, m->y);
    gfx_boxGrey(m->x, m->y, m->x + m->w, m->y + m->h, COLORTOGREY(COLOR_DEFAULT));

    for (u16 i = 0; m->__static.entries[i].type != ENTRY_END; i++) {
        if (m->cursorIndex == i) m->__static.entries[i].cursor = true;
        else                     m->__static.entries[i].cursor = false;

        _printEntry(m->__static.entries[i], 0);
    }

    SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
    gfx_con_setpos(0, SCREEN_HEIGHT - gfx_con.fntsz);
    // input_t* input = hidRead();
    // gfx_printf("Buttons active: %d", input->buttons);
    gfx_printf("Time taken for screen draw: %dus ", get_tmr_us() - lastDraw);
}

bool _can_cursor(entry_type_t type) {
    return (type == ENTRY_HANDLER || type == ENTRY_MENU || type == ENTRY_BACK || type == ENTRY_FILE || type == ENTRY_DIR);
}

void _handle_input(menu_t* m) {
    while (hidRead()) {
        if (RE_DETECTION(JOYPLUS)) {
            if (gfx_con.fntsz < 24) gfx_con_set_fontsz(gfx_con.fntsz += 1);
            break;
        }

        if (RE_DETECTION(JOYMINUS)) {
            if (gfx_con.fntsz > 8 ) gfx_con_set_fontsz(gfx_con.fntsz -= 1);
            break;
        }

        if (RE_DETECTION(JOYHOME)) {
            powerOff();
        }

        if (RE_DETECTION(JOYLUP)) {
            int prev = m->cursorIndex - 1;
            while (prev >= 0 && !_can_cursor(m->__static.entries[prev].type))
                prev--;

            if (prev >= 0) m->cursorIndex = prev;
            break;
        }

        if (RE_DETECTION(JOYLDOWN)) {
            int next = m->cursorIndex + 1;
            while (next < m->__static.count && !_can_cursor(m->__static.entries[next].type))
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
                push_menu(*(menu_t *)(entry.data));
                break;
            case ENTRY_BACK:
                pop_menu();
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

void menu_render_top() {
    menu_t* m = &menu_manager.stack[menu_manager.top];
    if (m->is_dynamic) { // TODO: Do way better.
        drawError(newError(TE_ERROR_NOT_IMPL_YET)); // TODO: do better.
        powerOff();
    }

    _render_static_menu(m);

    vic_compose();
    vic_wait_idle();

    _handle_input(m);
}


void pop_menu() {
    menu_t *m = &menu_manager.stack[menu_manager.top];
    if (m->is_dynamic) {
        vecFree(m->__dynamic.data);
    }

    memset(&menu_manager.stack[menu_manager.top], 0, sizeof(menu_t));
    menu_manager.top--;
}

void push_menu(menu_t m) {
    if (menu_manager.top++ >= MAX_STACK) drawError(newError(TE_ERROR_UNIMPLEMENTED)); // TODO: do better.
    menu_manager.stack[menu_manager.top] = m;
}
