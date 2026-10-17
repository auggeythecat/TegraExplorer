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

#include <utils/types.h>

typedef struct _teError_t {
    union {
        struct {
            u32 module:8;
            u32 code:24;
        };
        u32 error;
    };

    u32 accentColor;
    // TODO: Do I want more exotic colors? ie, purple?
    // Or, do I want to do something between red and yellow depending on the error?

    const char* moduleName;
} teError_t;

#define TE_MAKE_ERROR(error_module, error_code, color) \
((teError_t) {                                         \
    .module      = (error_module),                     \
    .code        = (error_code),                       \
    .accentColor = (color),                            \
    .moduleName  = (error_module##_NAME)               \
})

#define E_IS_SUCCESS(error) { (error == TE_SUCCESS) || (error.code == 0) }
#define E_IS_FAILURE(error) { (error != TE_SUCCESS) || (error.code != 0) }

#define      E_DISCARD(expr) { (void)(expr);         }
#define          E_SUCCESS() { E_RETURN(TE_SUCCESS); }

#define         E_SHOW_UNLESS(expr       ) { if ( const error e = (expr); E_IS_FAILURE(error) ) { drawError(error); } }
#define       E_RETURN_UNLESS(expr       ) { if ( const error e = (expr); E_IS_FAILURE(error) ) {  E_RETURN(error); } }
#define              E_ASSERT(expr       ) { if ( const error e = (expr); E_IS_FAILURE(error) ) { drawError(error); } }

#define E_RETURN_ERROR_UNLESS(expr, error) { if ( (expr) ) {  E_RETURN(error); } }
#define   E_SHOW_ERROR_UNLESS(expr, error) { if ( (expr) ) { drawError(error); } }
#define        E_ASSERT_ERROR(expr, error) { if ( (expr) ) { drawError(error); } }

#define         E_SHOW_UNLESS_ABORT(expr       ) {         E_SHOW_UNLESS(expr       ); powerOff(); }
#define       E_RETURN_UNLESS_ABORT(expr       ) {       E_RETURN_UNLESS(expr       ); powerOff(); }
#define              E_ASSERT_ABORT(expr       ) {              E_ASSERT(expr       ); powerOff(); }

#define E_RETURN_ERROR_UNLESS_ABORT(expr, error) { E_RETURN_ERROR_UNLESS(expr, error); powerOff(); }
#define   E_SHOW_ERROR_UNLESS_ABORT(expr, error) {   E_SHOW_ERROR_UNLESS(expr, error); powerOff(); }
#define        E_ASSERT_ERROR_ABORT(expr, error) {        E_ASSERT_ERROR(expr, error); powerOff(); }




/* Success */
#define TE_SUCCESS 0
//This shouldn't need a comment.

/* FatFS errors */
#define TE_ERROR_FATFS_MODULE 0
// Offset relative to zero for where FATFS error definitions start.

/* (sd) */
# define TE_ERROR_FATFS_SD_MODULE      0 + TE_ERROR_FATFS_MODULE // Module 0, 0x00
# define TE_ERROR_FATFS_SD_MODULE_NAME "Filesystem: SD"
// Offset relative to FATFS_OFFSET for where the SD error definitions start.
#  define TE_ERROR_SD_DISK_ERR            TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  1, COLOR_YELLOW_ORANGE) // Module 0, Error code  1
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_SD_INT_ERR             TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  2, COLOR_YELLOW_ORANGE) // Module 0, Error code  2
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_SD_NOT_READY           TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  3, COLOR_YELLOW_ORANGE) // Module 0, Error code  3
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_SD_NO_FILE             TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  4, COLOR_YELLOW_ORANGE) // Module 0, Error code  4
// The file could not be found at that path.
#  define TE_ERROR_SD_NO_PATH             TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  5, COLOR_YELLOW_ORANGE) // Module 0, Error code  5
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_SD_INVALID_NAME        TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  6, COLOR_YELLOW_ORANGE) // Module 0, Error code  6
// The string is not a valid path name.
#  define TE_ERROR_SD_DENIED              TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  7, COLOR_YELLOW_ORANGE) // Module 0, Error code  7
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_SD_EXIST               TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  8, COLOR_YELLOW_ORANGE) // Module 0, Error code  8
// The requested file/directory name already exists.
#  define TE_ERROR_SD_INVALID_OBJECT      TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE,  9, COLOR_YELLOW_ORANGE) // Module 0, Error code  9
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_SD_WRITE_PROTECTED     TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 10, COLOR_YELLOW_ORANGE) // Module 0, Error code 10
// The device is physically write-protected.
#  define TE_ERROR_SD_INVALID_DRIVE       TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 11, COLOR_YELLOW_ORANGE) // Module 0, Error code 11
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_SD_NOT_ENABLED         TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 12, COLOR_YELLOW_ORANGE) // Module 0, Error code 12
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_SD_NO_FILESYSTEM       TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 13, COLOR_YELLOW_ORANGE) // Module 0, Error code 13
// A FAT filesystem could not be found.
#  define TE_ERROR_SD_MKFS_ABORTED        TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 14, COLOR_YELLOW_ORANGE) // Module 0, Error code 14
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_SD_TIMEOUT             TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 15, COLOR_YELLOW_ORANGE) // Module 0, Error code 15
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_SD_LOCKED              TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 16, COLOR_YELLOW_ORANGE) // Module 0, Error code 16
// The operation to the object was rejected.
#  define TE_ERROR_SD_NOT_ENOUGH_CORE     TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 17, COLOR_YELLOW_ORANGE) // Module 0, Error code 17
// Not enough memory to carry out the action.
#  define TE_ERROR_SD_TOO_MANY_OPEN_FILES TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 18, COLOR_YELLOW_ORANGE) // Module 0, Error code 18
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_SD_INVALID_PARAMETER   TE_MAKE_ERROR(TE_ERROR_FATFS_SD_MODULE, 19, COLOR_YELLOW_ORANGE) // Module 0, Error code 19
// The function was called with invalid or inconsistent parameters.

/* (emuMMC) */
# define TE_ERROR_FATFS_EMU_MODULE      1 + TE_ERROR_FATFS_MODULE // Module 1, 0x01
# define TE_ERROR_FATFS_EMU_MODULE_NAME "Filesystem: emuMMC"
// Offset relative to FATFS_OFFSET for where the emuMMC error definitions start.
#  define TE_ERROR_EMU_DISK_ERR            TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  1, COLOR_YELLOW_ORANGE) // Module 1, Error code  1
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_EMU_INT_ERR             TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  2, COLOR_YELLOW_ORANGE) // Module 1, Error code  2
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_EMU_NOT_READY           TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  3, COLOR_YELLOW_ORANGE) // Module 1, Error code  3
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_EMU_NO_FILE             TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  4, COLOR_YELLOW_ORANGE) // Module 1, Error code  4
// The file could not be found at that path.
#  define TE_ERROR_EMU_NO_PATH             TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  5, COLOR_YELLOW_ORANGE) // Module 1, Error code  5
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_EMU_INVALID_NAME        TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  6, COLOR_YELLOW_ORANGE) // Module 1, Error code  6
// The string is not a valid path name.
#  define TE_ERROR_EMU_DENIED              TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  7, COLOR_YELLOW_ORANGE) // Module 1, Error code  7
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_EMU_EXIST               TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  8, COLOR_YELLOW_ORANGE) // Module 1, Error code  8
// The requested file/directory name already exists.
#  define TE_ERROR_EMU_INVALID_OBJECT      TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE,  9, COLOR_YELLOW_ORANGE) // Module 1, Error code  9
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_EMU_WRITE_PROTECTED     TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 10, COLOR_YELLOW_ORANGE) // Module 1, Error code 10
// The device is physically write-protected.
#  define TE_ERROR_EMU_INVALID_DRIVE       TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 11, COLOR_YELLOW_ORANGE) // Module 1, Error code 11
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_EMU_NOT_ENABLED         TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 12, COLOR_YELLOW_ORANGE) // Module 1, Error code 12
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_EMU_NO_FILESYSTEM       TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 13, COLOR_YELLOW_ORANGE) // Module 1, Error code 13
// A FAT filesystem could not be found.
#  define TE_ERROR_EMU_MKFS_ABORTED        TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 14, COLOR_YELLOW_ORANGE) // Module 1, Error code 14
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_EMU_TIMEOUT             TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 15, COLOR_YELLOW_ORANGE) // Module 1, Error code 15
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_EMU_LOCKED              TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 16, COLOR_YELLOW_ORANGE) // Module 1, Error code 16
// The operation to the object was rejected.
#  define TE_ERROR_EMU_NOT_ENOUGH_CORE     TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 17, COLOR_YELLOW_ORANGE) // Module 1, Error code 17
// Not enough memory to carry out the action.
#  define TE_ERROR_EMU_TOO_MANY_OPEN_FILES TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 18, COLOR_YELLOW_ORANGE) // Module 1, Error code 18
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_EMU_INVALID_PARAMETER   TE_MAKE_ERROR(TE_ERROR_FATFS_EMU_MODULE, 19, COLOR_YELLOW_ORANGE) // Module 1, Error code 19
// The function was called with invalid or inconsistent parameters.

/* (eMMC) */
# define TE_ERROR_FATFS_EMMC_MODULE      2 + TE_ERROR_FATFS_MODULE // Module 2, 0x02
# define TE_ERROR_FATFS_EMMC_MODULE_NAME "Filesystem: eMMC"
// Offset relative to FATFS_OFFSET for where the eMMC error definitions start.
#  define TE_ERROR_EMMC_DISK_ERR            TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  1, COLOR_YELLOW_ORANGE) // Module 2, Error code  1
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_EMMC_INT_ERR             TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  2, COLOR_YELLOW_ORANGE) // Module 2, Error code  2
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_EMMC_NOT_READY           TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  3, COLOR_YELLOW_ORANGE) // Module 2, Error code  3
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_EMMC_NO_FILE             TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  4, COLOR_YELLOW_ORANGE) // Module 2, Error code  4
// The file could not be found at that path.
#  define TE_ERROR_EMMC_NO_PATH             TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  5, COLOR_YELLOW_ORANGE) // Module 2, Error code  5
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_EMMC_INVALID_NAME        TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  6, COLOR_YELLOW_ORANGE) // Module 2, Error code  6
// The string is not a valid path name.
#  define TE_ERROR_EMMC_DENIED              TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  7, COLOR_YELLOW_ORANGE) // Module 2, Error code  7
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_EMMC_EXIST               TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  8, COLOR_YELLOW_ORANGE) // Module 2, Error code  8
// The requested file/directory name already exists.
#  define TE_ERROR_EMMC_INVALID_OBJECT      TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE,  9, COLOR_YELLOW_ORANGE) // Module 2, Error code  9
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_EMMC_WRITE_PROTECTED     TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 10, COLOR_YELLOW_ORANGE) // Module 2, Error code 10
// The device is physically write-protected.
#  define TE_ERROR_EMMC_INVALID_DRIVE       TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 11, COLOR_YELLOW_ORANGE) // Module 2, Error code 11
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_EMMC_NOT_ENABLED         TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 12, COLOR_YELLOW_ORANGE) // Module 2, Error code 12
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_EMMC_NO_FILESYSTEM       TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 13, COLOR_YELLOW_ORANGE) // Module 2, Error code 13
// A FAT filesystem could not be found.
#  define TE_ERROR_EMMC_MKFS_ABORTED        TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 14, COLOR_YELLOW_ORANGE) // Module 2, Error code 14
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_EMMC_TIMEOUT             TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 15, COLOR_YELLOW_ORANGE) // Module 2, Error code 15
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_EMMC_LOCKED              TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 16, COLOR_YELLOW_ORANGE) // Module 2, Error code 16
// The operation to the object was rejected.
#  define TE_ERROR_EMMC_NOT_ENOUGH_CORE     TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 17, COLOR_YELLOW_ORANGE) // Module 2, Error code 17
// Not enough memory to carry out the action.
#  define TE_ERROR_EMMC_TOO_MANY_OPEN_FILES TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 18, COLOR_YELLOW_ORANGE) // Module 2, Error code 18
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_EMMC_INVALID_PARAMETER   TE_MAKE_ERROR(TE_ERROR_FATFS_EMMC_MODULE, 19, COLOR_YELLOW_ORANGE) // Module 2, Error code 19
// The function was called with invalid or inconsistent parameters.

/* (ramdisk) */
# define TE_ERROR_FATFS_RAMDISK_MODULE      3 + TE_ERROR_FATFS_MODULE // Module 3, 0x03
# define TE_ERROR_FATFS_RAMDISK_MODULE_NAME "Filesystem: ramdisk"
// Offset relative to FATFS_OFFSET for where the ramdisk error definitions start.
#  define TE_ERROR_RAMDISK_DISK_ERR            TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  1, COLOR_YELLOW_ORANGE) // Module 3, Error code  1
// An unrecoverable hard error occurred in the disk access functions.
#  define TE_ERROR_RAMDISK_INT_ERR             TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  2, COLOR_YELLOW_ORANGE) // Module 3, Error code  2
// An assertion failed in an internal process, (stack overflow?).
#  define TE_ERROR_RAMDISK_NOT_READY           TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  3, COLOR_YELLOW_ORANGE) // Module 3, Error code  3
// The disk reported it wasn't ready during initialization.
#  define TE_ERROR_RAMDISK_NO_FILE             TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  4, COLOR_YELLOW_ORANGE) // Module 3, Error code  4
// The file could not be found at that path.
#  define TE_ERROR_RAMDISK_NO_PATH             TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  5, COLOR_YELLOW_ORANGE) // Module 3, Error code  5
// Could not follow the path, (invalid subdirectory?).
#  define TE_ERROR_RAMDISK_INVALID_NAME        TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  6, COLOR_YELLOW_ORANGE) // Module 3, Error code  6
// The string is not a valid path name.
#  define TE_ERROR_RAMDISK_DENIED              TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  7, COLOR_YELLOW_ORANGE) // Module 3, Error code  7
// The access was intentionally denied, (is something readonly?).
#  define TE_ERROR_RAMDISK_EXIST               TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  8, COLOR_YELLOW_ORANGE) // Module 3, Error code  8
// The requested file/directory name already exists.
#  define TE_ERROR_RAMDISK_INVALID_OBJECT      TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE,  9, COLOR_YELLOW_ORANGE) // Module 3, Error code  9
// The file or directory object is invalid, (is it null?).
#  define TE_ERROR_RAMDISK_WRITE_PROTECTED     TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 10, COLOR_YELLOW_ORANGE) // Module 3, Error code 10
// The device is physically write-protected.
#  define TE_ERROR_RAMDISK_INVALID_DRIVE       TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 11, COLOR_YELLOW_ORANGE) // Module 3, Error code 11
// The requested drive is nonexistent, or the given path is null.
#  define TE_ERROR_RAMDISK_NOT_ENABLED         TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 12, COLOR_YELLOW_ORANGE) // Module 3, Error code 12
// Work area not enabled, (did you call f_mount?).
#  define TE_ERROR_RAMDISK_NO_FILESYSTEM       TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 13, COLOR_YELLOW_ORANGE) // Module 3, Error code 13
// A FAT filesystem could not be found.
#  define TE_ERROR_RAMDISK_MKFS_ABORTED        TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 14, COLOR_YELLOW_ORANGE) // Module 3, Error code 14
// Making the filesystem aborted before it started, (is the drive big enough?)
#  define TE_ERROR_RAMDISK_TIMEOUT             TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 15, COLOR_YELLOW_ORANGE) // Module 3, Error code 15
// The function was stopped due to a thread-safe control.
#  define TE_ERROR_RAMDISK_LOCKED              TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 16, COLOR_YELLOW_ORANGE) // Module 3, Error code 16
// The operation to the object was rejected.
#  define TE_ERROR_RAMDISK_NOT_ENOUGH_CORE     TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 17, COLOR_YELLOW_ORANGE) // Module 3, Error code 17
// Not enough memory to carry out the action.
#  define TE_ERROR_RAMDISK_TOO_MANY_OPEN_FILES TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 18, COLOR_YELLOW_ORANGE) // Module 3, Error code 18
// The maximum number of objects has been reached. No more can be made.
#  define TE_ERROR_RAMDISK_INVALID_PARAMETER   TE_MAKE_ERROR(TE_ERROR_FATFS_RAMDISK_MODULE, 19, COLOR_YELLOW_ORANGE) // Module 3, Error code 19
// The function was called with invalid or inconsistent parameters.

/* Key derivation errors */
#define TE_ERROR_KEY_DERIVATION_MODULE      4 // Module 4, 0x04
#define TE_ERROR_KEY_DERIVATION_MODULE_NAME "Key/Cryptography"
// Offset relative to zero for where key derivation error definitions start.
/* TODO */

/* Lua errors */
#define TE_ERROR_LUA_MODULE      5 // Module 5, 0x05
#define TE_ERROR_LUA_MODULE_NAME "Lua"
// Offset relative to zero for where Lua error definitions start.
/* TODO: */

/* General errors */
#define TE_ERROR_GENERAL_MODULE      6 // Module 6, 0x06
#define TE_ERROR_GENERAL_MODULE_NAME "General"
// Offset relative to zero for where general error definitions start.
# define TE_ERROR_GENERAL_SD_MOUNT_FAILED     TE_MAKE_ERROR(TE_ERROR_GENERAL_MODULE, 1, COLOR_YELLOW_ORANGE) // Module 6, Error code 1
// Initial mounting of the sd card failed, (is it in the console?).
# define TE_ERROR_GENERAL_SD_READER_NOT_FOUND TE_MAKE_ERROR(TE_ERROR_GENERAL_MODULE, 2, COLOR_YELLOW_ORANGE) // Module 6, Error code 2
// The sd reader isn't inserted properly.
# define TE_ERROR_GENERAL_PAYLOAD_NOT_FOUND   TE_MAKE_ERROR(TE_ERROR_GENERAL_MODULE, 3, COLOR_YELLOW_ORANGE) // Module 6, Error code 3
// The requested payload file is not found.
# define TE_ERROR_GENERAL_PAYLOAD_TOO_LARGE   TE_MAKE_ERROR(TE_ERROR_GENERAL_MODULE, 4, COLOR_YELLOW_ORANGE) // Module 6, Error code 4
// The payload file attempted to be launched is too large.
# define TE_ERROR_GENERAL_VIC_TIMEOUT         TE_MAKE_ERROR(TE_ERROR_GENERAL_MODULE, 5, COLOR_YELLOW_ORANGE) // Module 6, Error code 5
// Time timeout was reached in VIC wait idle. Wait, how is this going to be displayed?

#define TE_ERROR_TEGRAEXPLORER_MODULE      7 // Module 7, 0x07
#define TE_ERROR_TEGRAEXPLORER_MODULE_NAME "Tegraexplorer"
// Offset relative to zero for where TegraExplorer error definitions start.
# define TE_ERROR_TEGRAEXPLORER_REACHED_BOTTOM_MENU TE_MAKE_ERROR(TE_ERROR_TEGRAEXPLORER_MODULE, 1, COLOR_YELLOW_ORANGE) // Module 7, Error code 1
// The menu system attempted to render the -1th menu, (is the main menu an overlay?).
# define TE_ERROR_TEGRAEXPLORER_POP_BOTTOM_MENU     TE_MAKE_ERROR(TE_ERROR_TEGRAEXPLORER_MODULE, 2, COLOR_YELLOW       ) // Module 7, Error code 2
// The main menu was attempted to be popped.
# define TE_ERROR_TEGRAEXPLORER_OUT_OF_MENUS        TE_MAKE_ERROR(TE_ERROR_TEGRAEXPLORER_MODULE, 2, COLOR_YELLOW_ORANGE) // Module 7, Error code 3
// There are too many menus loaded, no more can be.

/* Exceptions */
#define TE_EXCEPTION_MODULE      255 // Module 255, or 0xFF
#define TE_EXCEPTION_MODULE_NAME "Exception"
// Offset relative to zero for where exception definitions start.
# define TE_EXCEPTION_RESET              TE_MAKE_ERROR(TE_EXCEPTION_MODULE, 1, COLOR_RED) // Module 255, Error code 1
// Exits current execution and starts over from 0x00.
# define TE_EXCEPTION_UNDEFINED          TE_MAKE_ERROR(TE_EXCEPTION_MODULE, 2, COLOR_RED) // Module 255, Error code 2
// CPU encountered unknown/undecodable instruction.
# define TE_EXCEPTION_SOFTWARE_INTERRUPT TE_MAKE_ERROR(TE_EXCEPTION_MODULE, 3, COLOR_RED) // Module 255, Error code 3
// Software interrupt instruction reached.
# define TE_EXCEPTION_DATA_ABORT         TE_MAKE_ERROR(TE_EXCEPTION_MODULE, 4, COLOR_RED) // Module 255, Error code 4
// The requested memory access could not be fulfilled.
# define TE_EXCEPTION_PREFETCH_ABORT     TE_MAKE_ERROR(TE_EXCEPTION_MODULE, 5, COLOR_RED) // Module 255, Error code 5
// Failed to fetch the next instruction.

void drawError(teError_t error);
// TODO: Add more context (ie, pc, stack, etc.)

#endif //_ERROR_H
