//
// Created by auggiethecat
//

#include "../gfx/gfx.h"

#include "error.h"
#include "hid.h"

#define lx 256
#define ly 240
#define lenx 768
#define leny 240

const char* _TEErrors[] = {
    [TE_ERROR_IO]                     = "I/O error!",
    [TE_ERROR_NO_DISK]                = "No disk error!",
    [TE_ERROR_NOT_READY]              = "Not ready!",
    [TE_ERROR_NO_FILE]                = "No file present!",
    [TE_ERROR_NO_PATH]                = "No path present!",
    [TE_ERROR_PATH_INVALID]           = "Invalid path!",
    [TE_ERROR_ACCESS_DENIED]          = "Access is denied!",
    [TE_ERROR_ACCESS_DENIED_2]        = "Access is denied 2!",
    [TE_ERROR_INVALID_PTR]            = "Invalid pointer!",
    [TE_ERROR_PROTECTED]              = "Protected device!",
    [TE_ERROR_INVALID_DRIVE]          = "Invalid drive!",
    [TE_ERROR_NO_MEM]                 = "No memory left!",
    [TE_ERROR_NO_FAT]                 = "No FAT file system found!",
    [TE_ERROR_MKFS_ABORT]             = "MKFS aborted!",
    [TE_ERROR_UNIMPLEMENTED]          = "This feature isn't implemented yet!",
    [TE_ERROR_SAME_LOC]               = "Same file location!",
    [TE_ERROR_KEYDUMP_FAIL]           = "Failed keydump!",
    [TE_ERROR_PARTITION_NOT_FOUND]    = "This partition was not found!",
    [TE_ERROR_PATH_IN_PATH]           = "Path is in path!",
    [TE_ERROR_EMMC_READ_FAIL]         = "EMMC/EMUMMC read failed!",
    [TE_ERROR_EMMC_WRITE_FAIL]        = "EMMC/EMUMMC write failed!",
    [TE_ERROR_NO_SD]                  = "No SD found!",
    [TE_ERROR_FILE_TOO_BIG_FOR_DEST]  = "File too big for destination filesystem!",
    [TE_ERROR_MEM_ALLOC_FAIL]         = "Memory allocation failed!",

    [TE_EXCEPTION_RESET]              = "Exception: Reset!",
    [TE_EXCEPTION_UNDEFINED]          = "Exception: Undefined!",
    [TE_EXCEPTION_PREF_ABORT]         = "Exception: Pref abort!",
    [TE_EXCEPTION_DATA_ABORT]         = "Exception: Data abort!",

    [TE_WARN_FILE_EXISTS]             = "Warning: File already exists",
    [TE_WARN_FILE_TOO_SMALL_FOR_DEST] = "Warning: File is too small for destination!",
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
