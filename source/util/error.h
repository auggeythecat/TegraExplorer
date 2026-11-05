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
#define TE_SUCCESS 0
//This shouldn't need a comment.

/* FatFS errors */
#define TE_ERROR_FATFS_OFFSET 0
// Offset relative to zero for where FATFS error definitions start.

/* (sd) */
# define TE_ERROR_FATFS_SD_OFFSET          0 + TE_ERROR_FATFS_OFFSET
// Offset relative to FATFS_OFFSET for where the SD error definitions start.
#  define TE_ERROR_SD_DISK_ERR             1 + TE_ERROR_FATFS_SD_OFFSET // Error code  1
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_SD_INT_ERR              2 + TE_ERROR_FATFS_SD_OFFSET // Error code  2
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_SD_NOT_READY            3 + TE_ERROR_FATFS_SD_OFFSET // Error code  3
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_SD_NO_FILE              4 + TE_ERROR_FATFS_SD_OFFSET // Error code  4
// The file could not be found at that path.
#  define TE_ERROR_SD_NO_PATH              5 + TE_ERROR_FATFS_SD_OFFSET // Error code  5
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_SD_INVALID_NAME         6 + TE_ERROR_FATFS_SD_OFFSET // Error code  6
// The string is not a valid path name.
#  define TE_ERROR_SD_DENIED               7 + TE_ERROR_FATFS_SD_OFFSET // Error code  7
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_SD_EXIST                8 + TE_ERROR_FATFS_SD_OFFSET // Error code  8
// The requested file/directory name already exists.
#  define TE_ERROR_SD_INVALID_OBJECT       9 + TE_ERROR_FATFS_SD_OFFSET // Error code  9
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_SD_WRITE_PROTECTED     10 + TE_ERROR_FATFS_SD_OFFSET // Error code 10
// The device is physically write-protected.
#  define TE_ERROR_SD_INVALID_DRIVE       11 + TE_ERROR_FATFS_SD_OFFSET // Error code 11
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_SD_NOT_ENABLED         12 + TE_ERROR_FATFS_SD_OFFSET // Error code 12
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_SD_NO_FILESYSTEM       13 + TE_ERROR_FATFS_SD_OFFSET // Error code 13
// A FAT filesystem could not be found.
#  define TE_ERROR_SD_MKFS_ABORTED        14 + TE_ERROR_FATFS_SD_OFFSET // Error code 14
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_SD_TIMEOUT             15 + TE_ERROR_FATFS_SD_OFFSET // Error code 15
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_SD_LOCKED              16 + TE_ERROR_FATFS_SD_OFFSET // Error code 16
// The operation to the object was rejected.
#  define TE_ERROR_SD_NOT_ENOUGH_CORE     17 + TE_ERROR_FATFS_SD_OFFSET // Error code 17
// Not enough memory to carry out the action.
#  define TE_ERROR_SD_TOO_MANY_OPEN_FILES 18 + TE_ERROR_FATFS_SD_OFFSET // Error code 18
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_SD_INVALID_PARAMETER   19 + TE_ERROR_FATFS_SD_OFFSET // Error code 19
// The function was called with invalid or inconsistent parameters.

/* (emuMMC) */
# define TE_ERROR_FATFS_EMU_OFFSET          50 + TE_ERROR_FATFS_OFFSET
// Offset relative to FATFS_OFFSET for where the emuMMC error definitions start.
#  define TE_ERROR_EMU_DISK_ERR             1 + TE_ERROR_FATFS_EMU_OFFSET // Error code 51
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_EMU_INT_ERR              2 + TE_ERROR_FATFS_EMU_OFFSET // Error code 52
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_EMU_NOT_READY            3 + TE_ERROR_FATFS_EMU_OFFSET // Error code 53
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_EMU_NO_FILE              4 + TE_ERROR_FATFS_EMU_OFFSET // Error code 54
// The file could not be found at that path.
#  define TE_ERROR_EMU_NO_PATH              5 + TE_ERROR_FATFS_EMU_OFFSET // Error code 55
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_EMU_INVALID_NAME         6 + TE_ERROR_FATFS_EMU_OFFSET // Error code 56
// The string is not a valid path name.
#  define TE_ERROR_EMU_DENIED               7 + TE_ERROR_FATFS_EMU_OFFSET // Error code 57
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_EMU_EXIST                8 + TE_ERROR_FATFS_EMU_OFFSET // Error code 58
// The requested file/directory name already exists.
#  define TE_ERROR_EMU_INVALID_OBJECT       9 + TE_ERROR_FATFS_EMU_OFFSET // Error code 59
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_EMU_WRITE_PROTECTED     10 + TE_ERROR_FATFS_EMU_OFFSET // Error code 60
// The device is physically write-protected.
#  define TE_ERROR_EMU_INVALID_DRIVE       11 + TE_ERROR_FATFS_EMU_OFFSET // Error code 61
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_EMU_NOT_ENABLED         12 + TE_ERROR_FATFS_EMU_OFFSET // Error code 62
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_EMU_NO_FILESYSTEM       13 + TE_ERROR_FATFS_EMU_OFFSET // Error code 63
// A FAT filesystem could not be found.
#  define TE_ERROR_EMU_MKFS_ABORTED        14 + TE_ERROR_FATFS_EMU_OFFSET // Error code 64
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_EMU_TIMEOUT             15 + TE_ERROR_FATFS_EMU_OFFSET // Error code 65
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_EMU_LOCKED              16 + TE_ERROR_FATFS_EMU_OFFSET // Error code 66
// The operation to the object was rejected.
#  define TE_ERROR_EMU_NOT_ENOUGH_CORE     17 + TE_ERROR_FATFS_EMU_OFFSET // Error code 67
// Not enough memory to carry out the action.
#  define TE_ERROR_EMU_TOO_MANY_OPEN_FILES 18 + TE_ERROR_FATFS_EMU_OFFSET // Error code 68
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_EMU_INVALID_PARAMETER   19 + TE_ERROR_FATFS_EMU_OFFSET // Error code 69
// The function was called with invalid or inconsistent parameters.

/* (eMMC) */
# define TE_ERROR_FATFS_EMMC_OFFSET        100 + TE_ERROR_FATFS_OFFSET
// Offset relative to FATFS_OFFSET for where the eMMC error definitions start.
#  define TE_ERROR_EMMC_DISK_ERR             1 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 101
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_EMMC_INT_ERR              2 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 102
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_EMMC_NOT_READY            3 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 103
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_EMMC_NO_FILE              4 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 104
// The file could not be found at that path.
#  define TE_ERROR_EMMC_NO_PATH              5 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 105
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_EMMC_INVALID_NAME         6 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 106
// The string is not a valid path name.
#  define TE_ERROR_EMMC_DENIED               7 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 107
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_EMMC_EXIST                8 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 108
// The requested file/directory name already exists.
#  define TE_ERROR_EMMC_INVALID_OBJECT       9 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 109
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_EMMC_WRITE_PROTECTED     10 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 110
// The device is physically write-protected.
#  define TE_ERROR_EMMC_INVALID_DRIVE       11 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 111
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_EMMC_NOT_ENABLED         12 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 112
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_EMMC_NO_FILESYSTEM       13 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 113
// A FAT filesystem could not be found.
#  define TE_ERROR_EMMC_MKFS_ABORTED        14 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 114
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_EMMC_TIMEOUT             15 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 115
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_EMMC_LOCKED              16 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 116
// The operation to the object was rejected.
#  define TE_ERROR_EMMC_NOT_ENOUGH_CORE     17 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 117
// Not enough memory to carry out the action.
#  define TE_ERROR_EMMC_TOO_MANY_OPEN_FILES 18 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 118
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_EMMC_INVALID_PARAMETER   19 + TE_ERROR_FATFS_EMMC_OFFSET // Error code 119
// The function was called with invalid or inconsistent parameters.


/* (ramdisk) */
# define TE_ERROR_FATFS_RAMDISK_OFFSET        150 + TE_ERROR_FATFS_OFFSET // Error code 150
// Offset relative to FATFS_OFFSET for where the ramdisk error definitions start.
#  define TE_ERROR_RAMDISK_DISK_ERR             1 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 151
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_RAMDISK_INT_ERR              2 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 152
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_RAMDISK_NOT_READY            3 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 153
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_RAMDISK_NO_FILE              4 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 154
// The file could not be found at that path.
#  define TE_ERROR_RAMDISK_NO_PATH              5 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 155
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_RAMDISK_INVALID_NAME         6 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 156
// The string is not a valid path name.
#  define TE_ERROR_RAMDISK_DENIED               7 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 157
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_RAMDISK_EXIST                8 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 158
// The requested file/directory name already exists.
#  define TE_ERROR_RAMDISK_INVALID_OBJECT       9 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 159
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_RAMDISK_WRITE_PROTECTED     10 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 160
// The device is physically write-protected.
#  define TE_ERROR_RAMDISK_INVALID_DRIVE       11 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 161
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_RAMDISK_NOT_ENABLED         12 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 162
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_RAMDISK_NO_FILESYSTEM       13 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 163
// A FAT filesystem could not be found.
#  define TE_ERROR_RAMDISK_MKFS_ABORTED        14 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 164
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_RAMDISK_TIMEOUT             15 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 165
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_RAMDISK_LOCKED              16 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 166
// The operation to the object was rejected.
#  define TE_ERROR_RAMDISK_NOT_ENOUGH_CORE     17 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 167
// Not enough memory to carry out the action.
#  define TE_ERROR_RAMDISK_TOO_MANY_OPEN_FILES 18 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 168
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_RAMDISK_INVALID_PARAMETER   19 + TE_ERROR_FATFS_RAMDISK_OFFSET // Error code 169
// The function was called with invalid or inconsistent parameters.

/* Key derivation errors */
#define TE_ERROR_KEY_DERIVATION_OFFSET 200
// Offset relative to zero for where key derivation error definitions start.
/* TODO */

/* Lua errors */
#define TE_ERROR_LUA_OFFSET 300
// Offset relative to zero for where Lua error definitions start.
/* TODO: */

/* General errors */
#define TE_ERROR_GENERAL_OFFSET 400
// Offset relative to zero for where general error definitions start.
# define TE_ERROR_GENERAL_SD_MOUNT_FAILED 1 + TE_ERROR_GENERAL_OFFSET // Error code 401

#define TE_ERROR_TE_OFFSET 500
// Offset relative to zero for where TegraExplorer error definitions start.
# define TE_ERROR_TE_REACHED_BOTTOM_MENU 1 + TE_ERROR_TE_OFFSET // Error code 501
// The menu system attempted to render the -1th menu, (is the main menu an overlay?).
# define TE_ERROR_TE_OUT_OF_MENUS        2 + TE_ERROR_TE_OFFSET // Error code 502
// The main menu was attempted to be popped.

/* Exceptions */
#define TE_EXCEPTION_OFFSET 1000
// Offset relative to zero for where exception definitions start.
# define TE_EXCEPTION_RESET              1 + TE_EXCEPTION_OFFSET // Error code 1001

# define TE_EXCEPTION_UNDEFINED          2 + TE_EXCEPTION_OFFSET // Error code 1002
// CPU encountered unknown/undecodable instruction.
# define TE_EXCEPTION_SOFTWARE_INTERRUPT 3 + TE_EXCEPTION_OFFSET // Error code 1003
// Software interrupt instruction reached.
# define TE_EXCEPTION_DATA_ABORT         4 + TE_EXCEPTION_OFFSET // Error code 1004
// The requested memory access could not be fulfilled.
# define TE_EXCEPTION_PREF_ABORT         5 + TE_EXCEPTION_OFFSET // Error code 1005
//

#endif //_ERROR_H
