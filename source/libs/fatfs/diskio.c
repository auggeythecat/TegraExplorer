/*—————————————————————————————————————————————————————————————————————————————
 — Copyright (c) 2016 ChaN!                                                             —
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

/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <libs/fatfs/diskio.h>
#include <storage/sdmmc.h>
#include <storage/sd.h>
#include <storage/emmc.h>
#include <storage/nx_emmc_bis.h>
#include <storage/ramdisk.h>

static u32 sd_rsvd_sectors = 0;
static u32 emmc_sectors    = 0;
static u32 bis_sectors     = 0;
static u32 emummc_sectors  = 0;
static u32 ramdisk_sectors = 0;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
    BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    (void)(pdrv);
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
    BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    (void)(pdrv);
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
    BYTE pdrv,		/* Physical drive number to identify the drive */
    BYTE *buff,		/* Data buffer to store read data */
    DWORD sector,	/* Start sector in LBA */
    UINT count		/* Number of sectors to read */
)
{
    switch (pdrv) {
    case DRIVE_SD:
        return sdmmc_storage_read(&sd_storage, sector, count, (void*)buff);
    case DRIVE_EMMC:
        return sdmmc_storage_read(&emmc_storage, sector, count, (void*)buff);
    case DRIVE_RAM:
        return ram_disk_read(sector, count, (void*)buff);
    case DRIVE_BIS:
    case DRIVE_EMU:
        return nx_emmc_bis_read(sector, count, (void*)buff);
    }

    return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (
    BYTE pdrv,			/* Physical drive nmuber to identify the drive */
    const BYTE *buff,	/* Data to be written */
    DWORD sector,		/* Start sector in LBA */
    UINT count			/* Number of sectors to write */
)
{
    switch (pdrv) {
    case DRIVE_SD:
        return sdmmc_storage_write(&sd_storage, sector, count, (void*)buff);
    case DRIVE_EMMC:
        return sdmmc_storage_write(&emmc_storage, sector, count, (void*)buff);
    case DRIVE_RAM:
        return ram_disk_write(sector, count, (void*)buff);
    case DRIVE_BIS:
    case DRIVE_EMU:
        return nx_emmc_bis_write(sector, count, (void *)buff);
    }

    return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (
    BYTE pdrv,		/* Physical drive nmuber (0..) */
    BYTE cmd,		/* Control code */
    void *buff		/* Buffer to send/receive control data */
)
{
    DWORD *buf = (DWORD *)buff;

    switch (pdrv) {
    case DRIVE_SD:
        switch (cmd) {
        case GET_SECTOR_COUNT:
            *buf = sd_storage.sec_cnt - sd_rsvd_sectors;
            break;
        case GET_BLOCK_SIZE:
            *buf = 32768;
            break;
        }
        break;

    case DRIVE_EMMC:
        switch (cmd) {
        case GET_SECTOR_COUNT:
            *buf = emmc_sectors;
            break;
        case GET_BLOCK_SIZE:
            *buf = 16384;
            break;
        }
        break;

    case DRIVE_RAM:
        switch (cmd) {
        case GET_SECTOR_COUNT:
            *buf = ramdisk_sectors;
            break;
        case GET_BLOCK_SIZE:
            *buf = 2048; // Align to 1MB.
            break;
        }
        break;

    case DRIVE_BIS:
        switch (cmd) {
        case GET_SECTOR_COUNT:
            *buf = bis_sectors;
            break;
        case GET_BLOCK_SIZE:
            *buf = 32768; // Align to 16MB.
            break;
        }
        break;

    case DRIVE_EMU:
        switch (cmd) {
        case GET_SECTOR_COUNT:
            *buf = emummc_sectors;
            break;
        case GET_BLOCK_SIZE:
                *buf = 16384; // Align to 8MB (With BOOT0/1 data will be at 16MB BU).
                break;
        }
        break;

    default: // Catch all for unknown devices.
        switch (cmd) {
        case CTRL_SYNC:
            break;
        case GET_SECTOR_COUNT:
        case GET_BLOCK_SIZE:
            *buf = 0; // Zero value to force default or abort.
            break;
        }
        break;
    }

    return RES_OK;
}

DRESULT disk_set_info (
    BYTE pdrv,		/* Physical drive nmuber (0..) */
    BYTE cmd,		/* Control code */
    void *buff		/* Buffer to send/receive control data */
)
{
    DWORD *buf = (DWORD *)buff;

    if (cmd == SET_SECTOR_COUNT) {
        switch (pdrv) {
        case DRIVE_SD:
            sd_rsvd_sectors = *buf;
            break;

        case DRIVE_EMMC:
            emmc_sectors = *buf;
            break;

        case DRIVE_RAM:
            ramdisk_sectors = *buf;
            break;

        case DRIVE_BIS:
            bis_sectors = *buf;
            break;

        case DRIVE_EMU:
            emummc_sectors = *buf;
            break;
        }
    }

    return RES_OK;
}