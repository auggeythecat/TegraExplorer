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

#include "utils.h"

#include <string.h>
#include <soc/hw_init.h>
#include <storage/sd.h>
#include <utils/util.h>

#include "../gfx/gfx.h"


void powerOff()         { power_set_state(POWER_OFF_RESET);     }
void rebootRCM()        { power_set_state(REBOOT_RCM);          }
void rebootFuseBypass() { power_set_state(REBOOT_BYPASS_FUSES); }
void rebootOFW()        { power_set_state(POWER_OFF_REBOOT);    }

#define RELOC_META_OFF      0x7C
#define PATCHED_RELOC_SZ    0x94
#define PATCHED_RELOC_STACK 0x40007000
#define PATCHED_RELOC_ENTRY 0x40010000
#define EXT_PAYLOAD_ADDR    0xC0000000
#define RCM_PAYLOAD_ADDR    (EXT_PAYLOAD_ADDR + ALIGN(PATCHED_RELOC_SZ, 0x10))

static void _reloc_append(const u32 payloadDst, const u32 payloadSrc, const u32 payloadSize)
{
    memcpy((u8 *)payloadSrc, (u8 *)IPL_LOAD_ADDR, PATCHED_RELOC_SZ);

    volatile reloc_meta_t *relocator = (reloc_meta_t *)(payloadSrc + RELOC_META_OFF);

    relocator->start = payloadDst - ALIGN(PATCHED_RELOC_SZ, 0x10);
    relocator->stack = PATCHED_RELOC_STACK;
    relocator->end   = payloadDst + payloadSize;
    relocator->ep    = payloadDst;
}

void launchPayload(const char* path) {
    if (!path || !path[0])
        return;

    if (sd_mount())
        goto out;

    u32 size = 0;
    void *buf = sd_file_read(path, &size);
    if (!buf) {
        EPRINTFARGS("Payload file is missing!\n(%s)", path);
        goto out;
    }

    if (size > 0x30000) {
        EPRINTF("Payload is too big!");
        goto out;
    }

    sd_end();

    memcpy((void *)RCM_PAYLOAD_ADDR, buf, size);

    _reloc_append(PATCHED_RELOC_ENTRY, EXT_PAYLOAD_ADDR, ALIGN(size, 0x10));

    hw_deinit(false);

    void (*payload_ptr)() = (void *)EXT_PAYLOAD_ADDR;
    (*payload_ptr)();

    out:
        sd_unmount();
}