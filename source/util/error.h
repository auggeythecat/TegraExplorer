//
// Created by yes on 5/15/26.
//

#ifndef _ERROR_H
#define _ERROR_H

#include <bdk.h>

enum {
    TE_ERROR_IO = 1,
    TE_ERROR_NO_DISK,
    TE_ERROR_NOT_READY,
    TE_ERROR_NO_FILE,
    TE_ERROR_NO_PATH,
    TE_ERROR_PATH_INVALID,
    TE_ERROR_ACCESS_DENIED,
    TE_ERROR_ACCESS_DENIED_2, // TODO: Why is this exactly?
    TE_ERROR_INVALID_PTR,
    TE_ERROR_PROTECTED,
    TE_ERROR_INVALID_DRIVE,
    TE_ERROR_NO_MEM,
    TE_ERROR_NO_FAT,
    TE_ERROR_MKFS_ABORT,
    TE_ERROR_UNIMPLEMENTED,
    TE_ERROR_SAME_LOC,
    TE_ERROR_KEYDUMP_FAIL,
    TE_ERROR_PARTITION_NOT_FOUND,
    TE_ERROR_PATH_IN_PATH,
    TE_ERROR_EMMC_READ_FAIL,
    TE_ERROR_EMMC_WRITE_FAIL,
    TE_ERROR_NO_SD,
    TE_ERROR_FILE_TOO_BIG_FOR_DEST,
    TE_ERROR_MEM_ALLOC_FAIL,

    TE_EXCEPTION_RESET,
    TE_EXCEPTION_UNDEFINED,
    TE_EXCEPTION_PREF_ABORT,
    TE_EXCEPTION_DATA_ABORT,

    TE_WARN_FILE_EXISTS,
    TE_WARN_FILE_TOO_SMALL_FOR_DEST,
};


typedef struct _error_t {
    u16 code;
    u16 line;
    char *file;
} error_t;

#define newError(code) (error_t) {code, __LINE__, __FILE__}

void drawError(error_t error);
void garbage();

#endif //_ERROR_H
