//
// Created by yes on 5/15/26.
//

#include "error.h"
#include "../gfx/gfx.h"
#include "hid.h"

#define lx 256
#define ly 240
#define lenx 768
#define leny 240

const char* _TEErrors[] = {
    [TE_ERROR_IO]                     = "TMP",
    [TE_ERROR_NO_DISK]                = "TMP",
    [TE_ERROR_NOT_READY]              = "TMP",
    [TE_ERROR_NO_FILE]                = "TMP",
    [TE_ERROR_NO_PATH]                = "TMP",
    [TE_ERROR_PATH_INVALID]           = "TMP",
    [TE_ERROR_ACCESS_DENIED]          = "TMP",
    [TE_ERROR_ACCESS_DENIED_2]        = "TMP",
    [TE_ERROR_INVALID_PTR]            = "TMP",
    [TE_ERROR_PROTECTED]              = "TMP",
    [TE_ERROR_INVALID_DRIVE]          = "TMP",
    [TE_ERROR_NO_MEM]                 = "TMP",
    [TE_ERROR_NO_FAT]                 = "TMP",
    [TE_ERROR_MKFS_ABORT]             = "TMP",
    [TE_ERROR_UNIMPLEMENTED]          = "TMP",
    [TE_ERROR_SAME_LOC]               = "TMP",
    [TE_ERROR_KEYDUMP_FAIL]           = "TMP",
    [TE_ERROR_PARTITION_NOT_FOUND]    = "TMP",
    [TE_ERROR_PATH_IN_PATH]           = "TMP",
    [TE_ERROR_EMMC_READ_FAIL]         = "TMP",
    [TE_ERROR_EMMC_WRITE_FAIL]        = "TMP",
    [TE_ERROR_NO_SD]                  = "TMP",
    [TE_ERROR_FILE_TOO_BIG_FOR_DEST]  = "TMP",
    [TE_ERROR_MEM_ALLOC_FAIL]         = "TMP",

    [TE_EXCEPTION_RESET]              = "TMP",
    [TE_EXCEPTION_UNDEFINED]          = "TMP",
    [TE_EXCEPTION_PREF_ABORT]         = "TMP",
    [TE_EXCEPTION_DATA_ABORT]         = "TMP",

    [TE_WARN_FILE_EXISTS]             = "TMP",
    [TE_WARN_FILE_TOO_SMALL_FOR_DEST] = "TMP",
};

const char* _getErrorString(int error) {
    if (error > 0 && error <= ARRAY_SIZE(_TEErrors)) {
        return _TEErrors[error];
    }

    return "Unknown error!";
}

void drawError(error_t error) {
    if (error.code == 0)
        return;

    SETCOLOR(COLOR_ORANGE, COLOR_DARK_GREY);
    gfx_box(lx, ly, lx + lenx, ly + leny, COLOR_VIOLET);
    gfx_boxGrey(lx + 16, ly + 16, lx + lenx - 16, ly + leny - 16, 0x60);
    gfx_con_setpos(lx + ((lenx - 17 * 16) / 2), ly + 32);
    gfx_printf("An error occured!\n\n%bError: %d\nLine : %d\nFile : %s\nDesc : %s%b", lx + 48, error.code, error.line, error.file, _getErrorString(error.code), 0);
    gfx_con_setpos(lx + ((lenx - 19 * 16) / 2), ly + leny - 48);
    gfx_printf("Press A to continue");

    hidWaitMask((JoyA | JoyB));
}

void garbage() {
    u32 num = 0;
    num &= 0xFF000000;
    u32* buf = (u32*)malloc(4*1280*720);
    for (int x = 0; x < 720; x++) {
        for (int y = 0; y < 1280; y++) {
            num += 10;
            // gfx_printf("%d", num);
            buf[x+y] = num;
        }
    }

    gfx_set_rect_argb(buf, 720, 1280, 0, 0);
}
