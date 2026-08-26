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

typedef struct _menuEntry_t {
    const char* caption;
    u32 color;
    void* (*handler)(void*);
    u32 fileSize;

    union {
        struct {
            u32 skip:1;
            u32 hide:1;
            u32 selected:1;
            u32 selectable:1;

            u32 showIcon:1;
            u32 fileIcon:1;

            u32 filesizeIndex:2;

            u32 reserved:24;
        };
        u32 options;
    };
} menuEntry_t;

typedef struct _menu_t {
    const char* title;
    menuEntry_t* entries;
    u32 cursorIndex;
    u16 x, y, w, h;
} menu_t;

#define MAX_MENU_STACK 32
typedef struct _menuManager_t {
    menu_t stack[MAX_MENU_STACK];
    s8 top;
    bool fullRedraw;
} menuManager_t;

void  popMenu();
void pushMenu(menu_t m);

#endif //_MENU_H
