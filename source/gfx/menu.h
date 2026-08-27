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
    void* (*handler)(void*);
    u32 fileSize;

    union {
        struct {
            u32 renderDirty:1;

            u32 skip:1;
            u32 hide:1;
            u32 selected:1;
            u32 selectable:1;

            u32 showIcon:1;
            u32 fileIcon:1;

            u32 filesizeIndex:2;

            u32 reserved:23;
        };
        u32 options;
    };
} menuEntry_t;

#define        ENT_END(                             ) { ENTRY_END       , COLOR_NONE, 0      , NULL, NULL   , { .skip = 1, .hide = 1 } }
#define  ENT_SEPERATOR(                             ) { ENTRY_SEPERATOR , COLOR_NONE, 0      , NULL, NULL   , { .skip = 1, .hide = 1 } }
#define    ENT_CAPTION(color, caption               ) { ENTRY_CAPTION   , color     , caption, NULL, NULL   , { .skip = 1            } }
#define    ENT_HANDLER(color, caption,       handler) { ENTRY_HANDLER   , color     , caption, NULL, handler, { 0                    } }
#define ENT_HANDLER_EX(color, caption, data, handler) { ENTRY_HANDLER_EX, color     , caption, data, handler, { 0                    } }
#define       ENT_MENU(color, caption, data         ) { ENTRY_MENU      , color     , caption, data, NULL   , { 0                    } }
#define  ENT_DIRECTORY(color, caption, data, handler) { ENTRY_DIRECTORY , color     , caption, data, NULL   , { .showSize = 1        } }
#define       ENT_FILE(color, caption, data, handler) { ENTRY_FILE      , color     , caption, data, NULL   , { .showSize = 1        } }
#define       ENT_BACK(color, caption               ) { ENTRY_BACK      , color     , caption, NULL, NULL   , { 0                    } }

typedef struct _menu_t {
    const char* title;
    menuEntry_t* entries;
    u32 cursorIndex, count;
    u16 x, y, w, h;

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
    s8 top;
    bool fullRedraw;
} menuManager_t;

void  popMenu();
void pushMenu(menu_t m);
void renderMenuTop();

#endif //_MENU_H
