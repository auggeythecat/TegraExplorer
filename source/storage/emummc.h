/*
* Copyright (c) 2019-2021 CTCaer
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EMUMMC_H
#define EMUMMC_H

#include <bdk.h>

typedef enum _emummcType_t {
    EMUMMC_TYPE_NONE      = 0,
    EMUMMC_TYPE_PARTITION = 1,
    EMUMMC_TYPE_FILES     = 2,
} emummcType_t;

typedef enum _emummcMMC_t {
    EMUMMC_MMC_NAND = 0,
    EMUMMC_MMC_SD   = 1,
    EMUMMC_MMC_GC   = 2,
} emummcMMC_t;

typedef struct _emummcCFG_t {
    int   enabled;
    u64   sector;
    u32   id;
    char *path;
    char *nintendoPath;
    // Internal.
    char *emummcFileBasedPath;
    u32 fileBasedPartSize;
    u32 activePart;
    int fsVer;
} emummcCFG_t;

extern emummcCFG_t emuCFG;

void emummcLoadCFG();
bool emummcSetPath(char *path);
int  emummcStorageInitMMC();
int  emummcStorageEnd();
int  emummcStorageRead(u32 sector, u32 numSectors, void *buf);
int  emummcStorageWrite(u32 sector, u32 numSectors, void *buf);
int  emummcStorageSetMMCPartition(u32 partition);

#endif