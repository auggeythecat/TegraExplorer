//
// Created by yes on 6/27/26.
//

#include "safeStorage.h"

#include <bdk.h>

#include "../../util/error.h"
#include "../../util/timeout.h"

typedef struct _safeSdmmcStorageWrite_t {
    sdmmc_storage_t *storage;
    u32 sector;
    u32 numSectors;
    void *buf;
} safeSdmmcStorageWrite_t;

int _safeSdmmcStorageWrite(void* arg) {
    safeSdmmcStorageWrite_t *args = (safeSdmmcStorageWrite_t*) arg;
    return sdmmc_storage_write(args->storage, args->sector, args->numSectors, args->buf);
}

// TODO: This is not currently function
int safeSdmmcStorageWrite(sdmmc_storage_t *storage, u32 sector, u32 numSectors, void *buf) {
    safeSdmmcStorageWrite_t args = {
        .storage    = storage,
        .sector     = sector,
        .numSectors = numSectors,
        .buf        = buf,
    };

    return executeWithTimeout(500000, &_safeSdmmcStorageWrite, &args);
}

typedef struct _safeSdmmcStorageRead_t {
    sdmmc_storage_t *storage;
    u32 sector;
    u32 numSectors;
    void *buf;
} safeSdmmcStorageRead_t;

int _safeSdmmcStorageRead(void* arg) {
    safeSdmmcStorageRead_t *args = (safeSdmmcStorageRead_t*) arg;
    return sdmmc_storage_read(args->storage, args->sector, args->numSectors, args->buf);
}

int safeSdmmcStorageRead(sdmmc_storage_t *storage, u32 sector, u32 numSectors, void *buf) {
    safeSdmmcStorageRead_t args = {
        .storage    = storage,
        .sector     = sector,
        .numSectors = numSectors,
        .buf        = buf,
    };

    return executeWithTimeout(500000, &_safeSdmmcStorageRead, &args);
}

int iloop(void* null) {
    msleep(1000000);
    return 0;
}

void interuptTest() {
    executeWithTimeout(500000, &iloop, NULL);
}

