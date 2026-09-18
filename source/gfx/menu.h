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
#include "../configuration.h"
#include "colors.h"

typedef enum _entryType_t {
    ENTRY_END = 0,
    ENTRY_SEPARATOR,
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
    char* caption;
    u32 color;

#if UB_FUNCTION_POINTER_HACK
    // So...
    // Using functions with no args as the handler is technically
    // undefined behavior. (It will be called with the NULL arg).
    // After further review, it's *probably* fine for this usage,
    // but its still not something I like. Also, Hekate does use
    // this exactly, which does make me feel better, but I don't
    // *really* like this,
    void* data;
    void(*handler)(void*);
#else
    union {
        void (*handler_no_arg)(void);
        void (*handler_with_arg)(void*);
    } __handler;
#endif

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

            u32 captionStatic:1;
#if UB_FUNCTION_POINTER_HACK
            u32 reserved:11;
#else
            u32 handleWithArgs:1;
            u32 reserved:10;
#endif
        };
        u32 options;
    };
} menuEntry_t;

#ifdef UB_FUNCTION_POINTER_HACK
# define        ENT_END_OPTIONS { .renderDirty = 1, .captionStatic = 1, .skip = 1, .hide = 1                                     }
# define  ENT_SEPARATOR_OPTIONS { .renderDirty = 1, .captionStatic = 1, .skip = 1, .hide = 1                                     }
# define    ENT_CAPTION_OPTIONS { .renderDirty = 1, .captionStatic = 1, .skip = 1,                                               }
# define    ENT_HANDLER_OPTIONS { .renderDirty = 1, .captionStatic = 1,                                                          }
# define ENT_HANDLER_EX_OPTIONS { .renderDirty = 1, .captionStatic = 1,                                                          }
# define       ENT_MENU_OPTIONS { .renderDirty = 1, .captionStatic = 1,                                                          }
# define  ENT_DIRECTORY_OPTIONS { .renderDirty = 1, .captionStatic = 0, .showIcon = 1, .icon = 0, .selectable = 1                }
# define       ENT_FILE_OPTIONS { .renderDirty = 1, .captionStatic = 0, .showIcon = 1, .icon = 1, .selectable = 1, .showSize = 1 }
# define       ENT_BACK_OPTIONS { .renderDirty = 1, .captionStatic = 1,                                                          }
#else
# define        ENT_END_OPTIONS { .renderDirty = 1, .captionStatic = 1,                     .skip = 1, .hide = 1                                     }
# define  ENT_SEPARATOR_OPTIONS { .renderDirty = 1, .captionStatic = 1,                     .skip = 1, .hide = 1                                     }
# define    ENT_CAPTION_OPTIONS { .renderDirty = 1, .captionStatic = 1,                     .skip = 1,                                               }
# define    ENT_HANDLER_OPTIONS { .renderDirty = 1, .captionStatic = 1, handleWithArgs = 0                                                           }
# define ENT_HANDLER_EX_OPTIONS { .renderDirty = 1, .captionStatic = 1, handleWithArgs = 1                                                           }
# define       ENT_MENU_OPTIONS { .renderDirty = 1, .captionStatic = 1, handleWithArgs = 1                                                           }
# define  ENT_DIRECTORY_OPTIONS { .renderDirty = 1, .captionStatic = 0, handleWithArgs = 1, .showIcon = 1, .icon = 0, .selectable = 1                }
# define       ENT_FILE_OPTIONS { .renderDirty = 1, .captionStatic = 0, handleWithArgs = 1, .showIcon = 1, .icon = 1, .selectable = 1, .showSize = 1 }
# define       ENT_BACK_OPTIONS { .renderDirty = 1, .captionStatic = 1,                                                                              }
#endif

#define        ENT_END(                             ) (menuEntry_t) { ENTRY_END       , 0      , COLOR_NONE, NULL, NULL   , ENT_END_OPTIONS        }
#define  ENT_SEPARATOR(                             ) (menuEntry_t) { ENTRY_SEPARATOR , 0      , COLOR_NONE, NULL, NULL   , ENT_SEPARATOR_OPTIONS  }
#define    ENT_CAPTION(color, caption               ) (menuEntry_t) { ENTRY_CAPTION   , caption, color     , NULL, NULL   , ENT_CAPTION_OPTIONS    }
#define    ENT_HANDLER(color, caption,       handler) (menuEntry_t) { ENTRY_HANDLER   , caption, color     , NULL, handler, ENT_HANDLER_OPTIONS    }
#define ENT_HANDLER_EX(color, caption, data, handler) (menuEntry_t) { ENTRY_HANDLER_EX, caption, color     , data, handler, ENT_HANDLER_EX_OPTIONS }
#define       ENT_MENU(color, caption, data, handler) (menuEntry_t) { ENTRY_MENU      , caption, color     , data, handler, ENT_MENU_OPTIONS       }
#define  ENT_DIRECTORY(color, caption, data, handler) (menuEntry_t) { ENTRY_DIRECTORY , caption, color     , data, NULL   , ENT_DIRECTORY_OPTIONS  }
#define       ENT_FILE(color, caption, data, handler) (menuEntry_t) { ENTRY_FILE      , caption, color     , data, NULL   , ENT_FILE_OPTIONS       }
#define       ENT_BACK(color, caption               ) (menuEntry_t) { ENTRY_BACK      , caption, color     , NULL, NULL   , ENT_BACK_OPTIONS       }

typedef struct _menu_t {
    const char* title;
    menuEntry_t* entries;
    u16 cursorIndex, count;
    u16 x, y, w, h;

    union {
        struct {
            u32 isOverlay:1;
            u32 printHeader:1;
            u32 printFooter:1;

            u32 renderDirty:1;
            u32 titleStatic:1;
        };
        u32 options;
    };
} menu_t;

#define MAX_MENU_STACK 32
typedef struct _menuManager_t {
    menu_t stack[MAX_MENU_STACK];
    u32 top, lastDraw;
    bool fullRedraw;
} menuManager_t;

void  popMenu();
void pushMenu(menu_t m);
void renderMenuTop();

#endif //_MENU_H
