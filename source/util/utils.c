//
// Created by yes on 6/1/26.
//

#include "utils.h"

#include <string.h>
#include <bdk.h>

static void _reloc_append(u32 payload_dst, u32 payload_src, u32 payload_size)
{
    // memcpy((u8 *)payload_src, (u8 *)nyx_str->hekate, PATCHED_RELOC_SZ);

    volatile reloc_meta_t *relocator = (reloc_meta_t *)(payload_src + RELOC_META_OFF);

    relocator->start = payload_dst - ALIGN(PATCHED_RELOC_SZ, 0x10);
    relocator->stack = PATCHED_RELOC_STACK;
    relocator->end   = payload_dst + payload_size;
    relocator->ep    = payload_dst;
}


void launch_payload(char *path, bool clear_screen)
{
    if (clear_screen)
        gfx_clear_grey(0x1B);
    gfx_con_setpos(0, 0);

    u32 size = 0;
    void *buf = sd_file_read(path, &size);
    if (!buf) {
        gfx_con.mute = false;
        EPRINTFARGS("Payload file is missing!\n(%s)", path);

        goto out;
    }

    // if (update && is_ipl_updated(buf, size, path, false))
    //     goto out;

    if (size > 0x30000)
    {
        gfx_con.mute = false;
        EPRINTF("Payload is too big!");

        goto out;
    }

    sd_end();

    memcpy((void *)RCM_PAYLOAD_ADDR, buf, size);


    void (*payload_ptr)();
    payload_ptr = (void *)RCM_PAYLOAD_ADDR;

    hw_deinit(false);

    // Launch our payload.
    (*payload_ptr)();

    out:
    free(buf);
    gfx_con.mute = false;
    EPRINTF("Failed to launch payload!");

}

void powerOff() {
    power_set_state(POWER_OFF_RESET);
}

void rebootRCM() {
    power_set_state(REBOOT_RCM);
}

void reboot() {
    power_set_state(REBOOT_BYPASS_FUSES);
}