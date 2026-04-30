//
// Created by auggiethecat on 4/29/26.
//

#ifndef _CONFIG_H
#define _CONFIG_H

#include <bdk.h>

// !TODO: Add things as needed

typedef struct TEConfig_t {
    bool emummcForceDisable;
    bool minervaEnabled;

    u32 FSBuffSize;
    u32 errors;
    const char* Pkg1ID;
} TEConfig_t;

extern TEConfig_t TEConfig;

#endif //_CONFIG_H