//
// Created by yes on 6/27/26.
//

#ifndef _SAFESTORAGE_H
#define _SAFESTORAGE_H

#include <bdk.h>

int  safeSdmmcStorageWrite(sdmmc_storage_t *storage, u32 sector, u32 numSectors, void *buf);
int  safeSdmmcStorageRead (sdmmc_storage_t *storage, u32 sector, u32 numSectors, void *buf);
void interuptTest();

#endif //_SAFESTORAGE_H
