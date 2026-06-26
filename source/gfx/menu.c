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
    if (entry.type == ENTRY_SEPERATOR || entry.hide) {
        gfx_putc('\n');
        return;
    }

    if (entry.icon) {
        gfx_putc(entry.icon); gfx_putc(' ');
        maxLen -= 2;
    }

    u32 curX = 0, curY = 0;
    gfx_con_getpos(&curX, &curY);

    if (entry.type == ENTRY_DIR || entry.type == ENTRY_FILE || entry.type == ENTRY_HANDLER || entry.type == ENTRY_MENU || entry.type == ENTRY_BACK) {
        if (entry.cursor) SETCOLOR(INVERTCOLOR(entry.color), INVERTCOLOR(COLOR_BLACK));
        else              SETCOLOR(            entry.color , COLOR_BLACK);
    }

    if (!entry.showSize) {
        gfx_puts_limit(entry.caption, maxLen - 0);
        gfx_putc('\n');
    }
    else {
        gfx_puts_limit(entry.caption, maxLen - 8);
        gfx_con_setpos(curX + (maxLen - 6) * 16, curY);
        gfx_printf("%d", entry.showSize);
        gfx_puts_small(sizeDefs[entry.sizeIdx]);
    }
    SETCOLOR(COLOR_WHITE, COLOR_DEFAULT);
}

void inline _render_dynamic_menu(menu_t m) {

}

void _render_static_menu(menu_t* m) {
    u32 lastDraw = get_tmr_us();

    if (!m->is_overlay) // TODO: This will not work with small entry refreshing
        gfx_clear_color(COLORTOGREY(COLOR_DEFAULT));

    if (m->page_count) {
        SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
        char temp[40] = "";

        u16 items_per_page = m->h / gfx_con.fntsz;
        u16 total_pages = (m->__static.count / items_per_page) + 1;
        u16 current_page = (m->offset / items_per_page) + 1;

        s_printf(temp, " Page %d / %d | Total %d entries", current_page, total_pages, m->__static.count); // Figure out actual pages. This might be tricky with variable font sizes though.
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
    input_t* input = hidRead();
    // gfx_printf("Buttons active: %d", input->buttons);
    gfx_printf("Time taken for screen draw: %dus ", get_tmr_us() - lastDraw);
}

bool _is_selectable(entry_type_t type) {
    return (type == ENTRY_HANDLER || type == ENTRY_MENU || type == ENTRY_BACK || type == ENTRY_FILE || type == ENTRY_DIR);
}

void _handle_input(menu_t* m) {
    input_t *input = hidRead();

    while (hidRead()) {
        if (input->buttons & JoyPLUS) {
            if (gfx_con.fntsz < 24) gfx_con_set_fontsz(gfx_con.fntsz += 1);
            break;
        }

        if (input->buttons & JoyMINUS) {
            if (gfx_con.fntsz > 8 ) gfx_con_set_fontsz(gfx_con.fntsz -= 1);
            break;
        }

        if (input->buttons & JoyHOME) {
            powerOff();
        }

        if (input->buttons & JoyRUp) {
            int prev = m->cursorIndex - 1;
            while (prev >= 0 && !_is_selectable(m->__static.entries[prev].type)) {
                prev--;
            }
            if (prev >= 0) m->cursorIndex = prev;
            break;
        }

        if (input->buttons & JoyRDown) {
            int next = m->cursorIndex + 1;
            while (next < m->__static.count && !_is_selectable(m->__static.entries[next].type)) {
                next++;
            }
            if (next < m->__static.count) m->cursorIndex = next;
            break;
        }

        if (input->buttons & JoyA) {
            if (m->is_dynamic) {
                // TODO:
            } else {
                switch (m->__static.entries[m->cursorIndex].type) {
                    case ENTRY_HANDLER:
                        m->__static.entries[m->cursorIndex].handler(NULL);
                        break;
                    case ENTRY_MENU:
                        push_menu(*(menu_t *)(m->__static.entries[m->cursorIndex].data));
                        break;
                    case ENTRY_BACK:
                        pop_menu();
                        break;
                    default:
                }
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
