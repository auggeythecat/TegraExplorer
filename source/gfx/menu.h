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
    ENTRY_DIR,
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

typedef struct _menu_t {
    const char *title;
    u16 x, y, w, h;
    u16 idx, offset;

    u16 cursorIndex;
    struct {
        u16 isOverlay:1;
        u16 isDynamic:1;
        u16 isPageCount:1;
        u16 reserved:13;
        // what the hell am I going to do with 13 bits?
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

#define MAX_STACK 10
typedef struct _menuManager_t {
    menu_t stack[MAX_STACK];
    s8 top;
    bool fullRedraw;
} menuManager_t;

void  popMenu();
void pushMenu(menu_t m);
void menuRenderTop();

#endif //_MENU_H
