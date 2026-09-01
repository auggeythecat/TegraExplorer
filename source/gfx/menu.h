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

#ifndef _MENU_H
#define _MENU_H

#include <utils/types.h>
#include "colors.h"

typedef enum _entryType_t {
    ENTRY_END = 0,
    ENTRY_SEPERATOR,
    ENTRY_CAPTION,
    ENTRY_HANDLER,
    ENTRY_HANDLER_EX,
    ENTRY_MENU,
    ENTRY_DIRECTORY,
    ENTRY_FILE,
    ENTRY_BACK,
} entryType_t;

typedef struct _menuEntry_t {
    u32 type;
    const char* caption;
    u32 color;
    void (*handler)(void);
    void* data;

    union {
        struct {
            u32 renderDirty:1;

            u32 skip:1;
            u32 hide:1;
            u32 selected:1;
            u32 selectable:1;
            u32 highlighted:1;

            u32 showIcon:1;
            u32 icon:1;

            u32 showSize:1;
            u32 fileSizeIndex:2;
            u32 fileSize:10;
            // It only needs to be 10, since the max value is 4GiB,
            // and the longest value would be 1023M/K/iB

            u32 reserved:12;
        };
        u32 options;
    };
} menuEntry_t;

#define        ENT_END(                             ) { ENTRY_END       , 0      , COLOR_NONE, NULL   , NULL, { .renderDirty = 1, .skip = 1, .hide = 1           } }
#define  ENT_SEPERATOR(                             ) { ENTRY_SEPERATOR , 0      , COLOR_NONE, NULL   , NULL, { .renderDirty = 1, .skip = 1, .hide = 1           } }
#define    ENT_CAPTION(color, caption               ) { ENTRY_CAPTION   , caption, color     , NULL   , NULL, { .renderDirty = 1, .skip = 1                      } }
#define    ENT_HANDLER(color, caption,       handler) { ENTRY_HANDLER   , caption, color     , handler, NULL, { .renderDirty = 1,                                } }
#define ENT_HANDLER_EX(color, caption, data, handler) { ENTRY_HANDLER_EX, caption, color     , handler, data, { .renderDirty = 1, .selectable = 1                } }
#define       ENT_MENU(color, caption, data, handler) { ENTRY_MENU      , caption, color     , handler, data, { .renderDirty = 1, .selectable = 1                } }
#define  ENT_DIRECTORY(color, caption, data, handler) { ENTRY_DIRECTORY , caption, color     , NULL   , data, { .renderDirty = 1, .selectable = 1, .showSize = 1 } }
#define       ENT_FILE(color, caption, data, handler) { ENTRY_FILE      , caption, color     , NULL   , data, { .renderDirty = 1, .selectable = 1  .showSize = 1 } }
#define       ENT_BACK(color, caption               ) { ENTRY_BACK      , caption, color     , NULL   , NULL, { .renderDirty = 1, .selectable = 1                } }

typedef struct _menu_t {
    const char* title;
    menuEntry_t* entries;
    u16 cursorIndex, count;
    u16 x, y, w, h;
    u32 lastDraw;

    union {
        struct {
            u32 isOverlay:1;
            u32 printHeader:1;
            u32 printFooter:1;

            u32 headerDirty:1;
            u32 footerDirty:1;
            u32 renderDirty:1;
        };
        u32 options;
    };
} menu_t;

#define MAX_MENU_STACK 32
typedef struct _menuManager_t {
    menu_t stack[MAX_MENU_STACK];
    u32 top;
    bool fullRedraw;
} menuManager_t;

void  popMenu();
void pushMenu(menu_t m);
void renderMenuTop();

#endif //_MENU_H
