//
// Created by yes on 5/20/26.
//

#ifndef _MENU_H
#define _MENU_H

#include <bdk.h>
#include "../../util/vector.h"

typedef void (*menuPath_t)();

typedef struct _menuEntry_t {
    union {
        struct {
            u32 B:8, G:8, R:8;
            u32 skip:1, hide:1;
        };
        u32 optionUnion;
    };

    union {
        struct {
            u16 size:12, showSize:1, sizeDef:3;
        };
        u16 sizeUnion;
    };

    u8 icon;
    char *name;
} menuEntry_t;


#define ENABLEB BIT(0)
#define ENABLEPAGECOUNT BIT(1)
#define ALWAYSREDRAW BIT(2)
#define USELIGHTGREY BIT(3)

#define SKIPBIT BIT(24)
#define HIDEBIT BIT(25)
#define SKIPHIDEBITS 0x3000000

u32 newMenu(vector_t vec, int startIndex, int screenLenX, int screenLenY, u8 options,int entryCount);

#endif //_MENU_H
