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

#ifndef _ERROR_H
#define _ERROR_H

/* Success */
#define TE_SUCCESS                           0

#define TE_ERROR_FATFS_OFFSET              0
/* FatFS errors  (sd) */
# define TE_ERROR_SD_OFFSET                0 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_DISK_ERR             1 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_INT_ERR              2 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_NOT_READY            3 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_NO_FILE              4 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_NO_PATH              5 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_INVALID_NAME         6 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_DENIED               7 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_EXIST                8 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_INVALID_OBJECT       9 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_WRITE_PROTECTED     10 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_INVALID_DRIVE       11 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_NOT_ENABLED         12 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_NO_FILESYSTEM       13 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_MKFS_ABORTED        14 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_TIMEOUT             15 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_LOCKED              16 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_NOT_ENOUGH_CORE     17 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_SD_TOO_MANY_OPEN_FILES 18 + TE_ERROR_FATFS_OFFSET

/* FatFS errors (eMMC) */
# define TE_ERROR_EMMC_OFFSET               18 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_DISK_ERR            19 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_INT_ERR             20 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_NOT_READY           21 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_NO_FILE             22 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_NO_PATH             23 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_INVALID_NAME        24 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_DENIED              25 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_EXIST               26 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_INVALID_OBJECT      27 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_WRITE_PROTECTED     28 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_INVALID_DRIVE       29 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_NOT_ENABLED         30 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_NO_FILESYSTEM       31 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_MKFS_ABORTED        32 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_TIMEOUT             33 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_LOCKED              34 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_NOT_ENOUGH_CORE     35 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMMC_TOO_MANY_OPEN_FILES 36 + TE_ERROR_FATFS_OFFSET


/* FatFS errors (emuMMC) */
# define TE_ERROR_EMU_OFFSET               36 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_DISK_ERR            37 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_INT_ERR             38 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_NOT_READY           39 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_NO_FILE             40 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_NO_PATH             41 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_INVALID_NAME        42 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_DENIED              43 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_EXIST               44 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_INVALID_OBJECT      45 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_WRITE_PROTECTED     46 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_INVALID_DRIVE       47 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_NOT_ENABLED         48 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_NO_FILESYSTEM       49 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_MKFS_ABORTED        50 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_TIMEOUT             51 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_LOCKED              52 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_NOT_ENOUGH_CORE     53 + TE_ERROR_FATFS_OFFSET
#  define TE_ERROR_EMU_TOO_MANY_OPEN_FILES 54 + TE_ERROR_FATFS_OFFSET


/* Exceptions */
#define TE_EXCEPTION_OFFSET      500                     // Error code 500;
# define TE_EXCEPTION_RESET      0 + TE_EXCEPTION_OFFSET // Error code 500;
# define TE_EXCEPTION_UNDEFINED  1 + TE_EXCEPTION_OFFSET // Error code 500;
# define TE_EXCEPTION_PREF_ABORT 2 + TE_EXCEPTION_OFFSET // Error code 500;
# define TE_EXCEPTION_DATA_ABORT 3 + TE_EXCEPTION_OFFSET // Error code 500;

#endif // _ERROR_H
