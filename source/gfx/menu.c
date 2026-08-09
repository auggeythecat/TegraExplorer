#include "menu.h"

#include <bdk.h>
#include <string.h>

#include "../util/error.h"
#include "../util/vector.h"
#include "../util/hid.h"
#include "gfx.h"
#include "../menus/explorers.h"
#include "../util/utils.h"

const char *sizeDefs[] = {
    "B  ",
    "KiB",
    "MiB",
    "GiB"
};

#define COLOR_FG COLOR_WHITE
#define COLOR_BG COLOR_DARKER_GREY

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
    case ENTRY_HANDLER_EX:
    case ENTRY_MENU:
    case ENTRY_BACK:
    case ENTRY_DIRECTORY: // TODO:
    case ENTRY_FILE: // TODO:
        SETCOLOR(COLOR_FG, COLOR_BG);
        if (entry.icon) {
            gfxPutc(entry.icon); gfxPutc('|');
            maxLen -= gfx_con.fntsz * 2;
        }

        u32 curX = 0, curY = 0;
        gfxConGetPos(&curX, &curY);

             if (entry.cursor)   SETCOLOR(INVERTCOLOR(entry.color), INVERTCOLOR(COLOR_BG));
        else if (entry.selected) SETCOLOR(            entry.color , INVERTCOLOR(COLOR_BG));
        else                     SETCOLOR(            entry.color ,             COLOR_BG );

        if (!entry.showSize) {
            gfxPutsLimit(entry.caption, maxLen - 0);
        } else {
            u32 numberSize = log_10(entry.size);
            gfxPutsLimit(entry.caption, maxLen - (numberSize * gfx_con.fntsz));
            gfxConSetPos(curX + (maxLen - (strlen(sizeDefs[entry.sizeIdx]) * 8)), curY);
            gfx_printf("%d", entry.size);
            gfxPutsSmall(sizeDefs[entry.sizeIdx]);
        }

        gfxPutc('\n');
        SETCOLOR(COLOR_FG, COLOR_BG);
        return;
    }
}

void _renderDynamicMenu(menu_t* m) {
    gfxConSetPos(1, 1);
    u32 lastDraw = get_tmr_us();

    if (!m->isOverlay) // TODO: This will not work with small entry refreshing
        gfxClearColor(COLOR_BG);

    u32 res; // TODO: Error handling
    if (m->isFileBrowser) {
        entry_t* entries = newVec(entry_t, 16);
        vecPushBack(entries, ((entry_t) ENT_SEPERATOR()));
        vecPushBack(entries, ((entry_t) ENT_CAPTION(COLOR_GREEN, m->__dynamic.path)));
        vecPushBack(entries, ((entry_t) ENT_BACK(COLOR_ORANGE, "<- Back")));

        DIR dir;
        FILINFO fno;

        res = f_opendir(&dir, m->__dynamic.path);

        if (res) { drawError(newError(TE_ERROR_IO)); }

        for (;;) {
            if (f_readdir(&dir, &fno) != FR_OK) { drawError(newError(TE_ERROR_IO)); break; }
            if (fno.fname[0] == '\0')            {                                  break; }
            if (fno.fname[0] == '.') continue;

            bool isDir = fno.fattrib & AM_DIR;

            u32 totalSize = fno.fsize;
            u32 sizeIdx = 0;
            if (!isDir) {
                while (totalSize > 1024){
                    totalSize /= 1024;
                    sizeIdx++;

                    if (sizeIdx >= 4)
                        break;
                }
            }

            char* filename = malloc(strlen(fno.fname) + 1);
            char* filepath = malloc(strlen(m->__dynamic.path) + strlen(fno.fname) + 1 + 1);

            if (!filename || !filepath) {
                f_closedir(&dir);
                drawError(newError(TE_ERROR_MEM_ALLOC_FAIL));
                return;
            }

            strcpy(filename, fno.fname);
            strcpy(filepath, m->__dynamic.path);
            strcat(filepath, "/");
            strcat(filepath, fno.fname);

            entry_t newEntry = {
                .type    = isDir ? ENTRY_DIRECTORY : ENTRY_FILE  ,
                .color   = isDir ? COLOR_FG     : COLOR_VIOLET,
                .caption = filename,
                .data    = filepath,
                .handler = isDir ? pushFolderMenu : NULL, // TODO: make a file menu.

                {
                .size     = totalSize,
                .sizeIdx  = sizeIdx,
                .icon     = isDir ? GFX_CHAR_FOLDER : GFX_CHAR_FILE,
                .selected = false,
                .cursor   = false,
                .skip     = false,
                .hide     = false,
                .showSize = isDir ? false : true,
                // .reserved = false
                }
            };

            filename = "";

            vecPushBack(entries, newEntry);
        }
        m->__dynamic.entries = entries;

        gfxConSetPos(m->x, m->y);
        for (u16 i = 0; entries[i].type != ENTRY_END; i++) {
            if (m->cursorIndex == i) entries[i].cursor = true;
            else                     entries[i].cursor = false;

            _printEntry(entries[i], 1000);
        }

        f_closedir(&dir);
    }

    if (m->isPageCount) {
        gfxConSetPos(0, 0);
        SETCOLOR(COLOR_BG, COLOR_FG);
        gfxBox(0, 0, SCREEN_WIDTH, gfx_con.fntsz, COLOR_FG);
        gfx_printf("TegraExplorer %s", TE_VER);
        char temp[40] = "";

        u32 count = vecGetCount(m->__dynamic.entries);

        u16 itemsPerPage = (m->h      / gfx_con.fntsz)    ;
        u16 totalPages   = (count     / itemsPerPage ) + 1;
        u16 currentPage  = (m->offset / itemsPerPage ) + 1;

        s_printf(temp, " Page %d / %d | %d entries", currentPage, totalPages, count);
        gfxConSetPos(SCREEN_WIDTH - (strlen(temp) * gfx_con.fntsz), 0);
        gfx_printf(temp);
    }

    SETCOLOR(COLOR_BG, COLOR_FG);
    gfxConSetPos(0, SCREEN_HEIGHT - gfx_con.fntsz);
    // gfx_printf("Buttons active: %d", inputs->buttons);
    gfx_printf("Time taken for screen draw: %dus ", get_tmr_us() - lastDraw);
    // gfx_printf("Item properties: %d", ((entry_t*) m->__dynamic.entries)[m->cursorIndex].information);
    // gfx_printf("Filepath of current item: %s", ((entry_t*)m->__dynamic.entries)[m->cursorIndex].data);
}

void _renderStaticMenu(menu_t* m) {
    gfxConSetPos(0, 0);
    u32 lastDraw = get_tmr_us();

    if (!m->isOverlay)
        gfxClearColor(COLOR_BG);

    if (m->isPageCount) {
        SETCOLOR(COLOR_BG, COLOR_FG);
        gfxBox(0, 0, SCREEN_WIDTH, (int) gfx_con.fntsz, COLOR_FG);
        gfxPuts("TegraExplorer 5.0.0");
        char temp[40] = "";

        u16 itemsPerPage = (m->h              / gfx_con.fntsz)    ;
        u16 totalPages   = (m->__static.count / itemsPerPage ) + 1;
        u16 currentPage  = (m->offset         / itemsPerPage ) + 1;

        s_printf(temp, " Page %d / %d | %d entries", currentPage, totalPages, m->__static.count);
        gfxConSetPos(SCREEN_WIDTH - (strlen(temp) * gfx_con.fntsz), 0);
        gfx_printf(temp);
    }

    gfxConSetPos(m->x, m->y);
    gfxBoxGrey(m->x, m->y, m->x + m->w, m->y + m->h, ~0x30);

    for (u16 i = 0; m->__static.entries[i].type != ENTRY_END; i++) {
        if (m->cursorIndex == i) m->__static.entries[i].cursor = true;
        else                     m->__static.entries[i].cursor = false;

        _printEntry(m->__static.entries[i], 9999999);
    }

    SETCOLOR(COLOR_BG, COLOR_FG);
    gfxConSetPos(0, SCREEN_HEIGHT - gfx_con.fntsz);
    // gfx_printf("Buttons active: %d", inputs->buttons);
    gfx_printf("Time taken for screen draw: %dus ", get_tmr_us() - lastDraw);
    // gfx_printf("Item properties: %d", m->__static.entries[m->cursorIndex].information);
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
            if (m->isDynamic) {
                while (prev >= 0 && ((entry_t*) m->__dynamic.entries)[prev].skip || ((entry_t*) m->__dynamic.entries)[prev].hide)
                    prev--;
            } else {
                while (prev >= 0 && (m->__static.entries[prev].skip || m->__static.entries[prev].hide))
                    prev--;
            }

            if (prev >= 0) m->cursorIndex = prev;
            break;
        }

        if (RE_DETECTION(JOYLDOWN)) {
            int next = m->cursorIndex + 1;
            if (m->isDynamic) {
                while (next < vecGetCount(m->__dynamic.entries) && ((entry_t*) m->__dynamic.entries)[next].skip || ((entry_t*) m->__dynamic.entries)[next].hide)
                    next++;

                if (next < vecGetCount(m->__dynamic.entries)) m->cursorIndex = next;
                break;
            } else {
                while (next < m->__static.count && (m->__static.entries[next].skip || m->__static.entries[next].hide))
                    next++;

                if (next < m->__static.count) m->cursorIndex = next;
                break;
            }
        }

        if (RE_DETECTION(JOYA)) {
            entry_t entry;
            if (m->isDynamic) entry = ((entry_t*)m->__dynamic.entries)[m->cursorIndex];
            else              entry =            m->__static.entries[m->cursorIndex];

            switch (entry.type) {
            case ENTRY_HANDLER:
                entry.handler(NULL);
                break;
            case ENTRY_HANDLER_EX:
                entry.handler(entry.data);
                break;
            case ENTRY_MENU:
                pushMenu(*(menu_t *)(entry.data));
                break;
            case ENTRY_BACK:
                popMenu();
                break;
            case ENTRY_FILE:
            case ENTRY_DIRECTORY:
                entry.handler(entry.data);
                break;
            default:
            }
            break;
        }

        if (RE_DETECTION(JOYB)) {
            popMenu();
            break;
        }
    }
}

void menuRenderTop() {
    menu_t* m = &menuManager.stack[menuManager.top];

    m->isDynamic ? _renderDynamicMenu(m) : _renderStaticMenu(m);

    vic_compose();
    vic_wait_idle();

    _handleInput(m);
}


void popMenu() {
    if (menuManager.top < 2) return; // Turns out, you can pop the main menu!

    menu_t *m = &menuManager.stack[menuManager.top];
    if (m->isDynamic) {
        if (m->__dynamic.path)
            free(m->__dynamic.path);

        if (!m->__dynamic.entries)
            goto clean;

        for (u32 i = 0; ((entry_t*)m->__dynamic.entries)[i].type != ENTRY_END; i++) {
            entry_t* entry = &((entry_t*)m->__dynamic.entries)[i];
            if (entry->caption && (entry->type == ENTRY_DIRECTORY || entry->type == ENTRY_FILE))
                free(entry->caption);
        }
    }

clean:
    memset(&menuManager.stack[menuManager.top], 0, sizeof(menu_t));
    menuManager.top--;
}

void pushMenu(menu_t m) {
    if (menuManager.top++ >= MAX_STACK) drawError(newError(TE_ERROR_UNIMPLEMENTED)); // TODO: do better.
    menuManager.stack[menuManager.top] = m;
}
