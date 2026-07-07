//
// Created by yes on 5/20/26.
//

#ifndef _MENU_H
#define _MENU_H

#include <bdk.h>

#include "../util/vector.h"

typedef enum _entryType_t {
    ENTRY_END = 0,
    ENTRY_SEPERATOR,
    ENTRY_CAPTION,
    ENTRY_HANDLER,
    ENTRY_MENU,
    ENTRY_DIRECTORY,
    ENTRY_FILE,
    ENTRY_BACK,
} entryType_t;

typedef struct _entry_t {
    entryType_t type;
    u32 color;
    const char *caption;
    void *data;
    void (*handler)(void *arg);

    union {
        struct {
            u32 size:16;
            u32 sizeIdx:2;
            u32 icon:8;

            u32 selected:1;
            u32 cursor:1;
            u32 skip:1;
            u32 hide:1;
            u32 showSize:1;
            u32 reserved:1;
        };
        u32 information;
    };
} entry_t;

#define SELECTED BIT(27)
#define CURSOR   BIT(28)
#define SKIP     BIT(29)
#define HIDE     BIT(30)
#define SHOWSIZE BIT(31)
// #define RESERVED BIT(32)

#define       ENT_END(                             ) { ENTRY_END      , COLOR_NONE, 0      , NULL, NULL   , { .skip = 1, .hide = 1 } }
#define ENT_SEPERATOR(                             ) { ENTRY_SEPERATOR, COLOR_NONE, 0      , NULL, NULL   , { .skip = 1, .hide = 1 } }
#define   ENT_CAPTION(color, caption               ) { ENTRY_CAPTION  , color     , caption, NULL, NULL   , { .skip = 1}             }
#define   ENT_HANDLER(color, caption,       handler) { ENTRY_HANDLER  , color     , caption, NULL, handler, { 0 }                    }
#define      ENT_MENU(color, caption, data         ) { ENTRY_MENU     , color     , caption, data, NULL   , { 0 }                    }
#define ENT_DIRECTORY(color, caption, data, handler) { ENTRY_FILE     , color     , caption, data, NULL   , { .showSize = 1 }        }
#define      ENT_FILE(color, caption, data, handler) { ENTRY_DIRECTORY, color     , caption, data, NULL   , { .showSize = 1 }        }
#define      ENT_BACK(color, caption               ) { ENTRY_BACK     , color     , 0      , NULL, NULL   , { 0 }                    }

typedef struct _menu_t {
    const char *title;
    u16 x, y, w, h;
    u16 idx, offset;

    struct {
        u32 isOverlay:1;
        u32 isDynamic:1;
        u32 isPageCount:1;
        u32 isFileBrowser:1;
        u32 cursorIndex:28; // TODO: Maybe find a better place for this that isn't here?
    };

    union {
        struct {
            entry_t* entries;
            u32 count;
        } __static;

        struct {
            u32 (*get_count)(void* ctx);
            void (*get_entries)(void* ctx, u32 idx, entry_t *out_entries);
            vector_t data;
        } __dynamic;
    };
} menu_t;

#define MAX_STACK 255
typedef struct _menuManager_t {
    menu_t stack[MAX_STACK];
    s8 top;
    bool fullRedraw;
} menuManager_t;

void  popMenu();
void pushMenu(menu_t m);
void menuRenderTop();

#endif //_MENU_H
