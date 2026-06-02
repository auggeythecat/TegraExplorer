//
// Created by yes on 6/1/26.
//

#ifndef _UTILS_H
#define _UTILS_H

#include <bdk.h>

#define RELOC_META_OFF      0x7C
#define PATCHED_RELOC_SZ    0x94
#define PATCHED_RELOC_STACK 0x40007000
#define PATCHED_RELOC_ENTRY 0x40010000
#define EXT_PAYLOAD_ADDR    0xC0000000
#define RCM_PAYLOAD_ADDR    (EXT_PAYLOAD_ADDR + ALIGN(PATCHED_RELOC_SZ, 0x10))

void launch_payload(char *path, bool clear_screen);

void powerOff();
void rebootRCM();
void reboot();

u32 fuse_read_bootrom_rev();

#endif //_UTILS_H
