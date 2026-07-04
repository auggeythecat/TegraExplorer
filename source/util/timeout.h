//
// Created by yes on 6/29/26.
//

#ifndef _TIMEOUT_H
#define _TIMEOUT_H

#include <bdk.h>

typedef int (*workPointer)(void* args);

typedef struct _saveState_t {
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 r8;
    u32 r9;
    u32 r10;
    u32 r11;
    u32 r12;
    u32 sp;
    u32 lr;
    u32 cpsr;
} saveState_t;

int executeWithTimeout(u32 timeoutUS, workPointer worker, void* args);

#endif //_TIMEOUT_H
