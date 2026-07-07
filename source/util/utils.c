//
// Created by yes on 6/1/26.
//

#include "utils.h"

#include <string.h>
#include <bdk.h>

static void _relocAppend(u32 payloadDst, u32 payloadSrc, u32 payloadSize)
{
    // memcpy((u8 *)payloadSrc, (u8 *)nyx_str->hekate, PATCHED_RELOC_SZ);

    volatile reloc_meta_t *relocator = (reloc_meta_t *)(payloadSrc + RELOC_META_OFF);

    relocator->start = payloadDst - ALIGN(PATCHED_RELOC_SZ, 0x10);
    relocator->stack = PATCHED_RELOC_STACK;
    relocator->end   = payloadDst + payloadSize;
    relocator->ep    = payloadDst;
}


void launchPayload(char *path, bool clearScreen)
{
    if (clearScreen)
        gfxClearGrey(0x1B);
    gfxConSetPos(0, 0);

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


    void (*payloadPtr)();
    payloadPtr = (void *)RCM_PAYLOAD_ADDR;

    hw_deinit(false);

    // Launch our payload.
    (*payloadPtr)();

    out:
    free(buf);
    gfx_con.mute = false;
    EPRINTF("Failed to launch payload!");

}

void powerOff()     { power_set_state(POWER_OFF_RESET    ); }
void rebootRCM()    { power_set_state(REBOOT_RCM         ); }
void reboot()       { power_set_state(REBOOT_BYPASS_FUSES); }

void rebootAMS()    { launchPayload("sd:/atmosphere/reboot_payload.bin", true); }
void rebootHekate() { launchPayload("sd:/bootloader/update.bin",         true); }

u32 fuseReadBootromRevision() {
    if (hw_get_chip_id() == GP_HIDREV_MAJOR_T210)
        return FUSE(FUSE_SOC_SPEEDO_1_CALIB);

    return FUSE(FUSE_SOC_SPEEDO_1_CALIB) | (1 << 12);
}