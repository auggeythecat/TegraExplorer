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

#include "sdMenu.h"

#include <libs/fatfs/ff.h>
#include <mem/heap.h>
#include <storage/sd.h>
#include <string.h>
#include <utils/sprintf.h>
#include <utils/types.h>

#include "../gfx/menu.h"
#include "../gfx/gfx.h"
#include "../util/vector.h"

static vector_t _listDirs(const char* path) {
    int res = 0;
    DIR dir;
    FILINFO fno;

    FILINFO* files = newVec(FILINFO, 16);
    if (!files) return NULL;

    sd_mount();

    res = f_opendir(&dir, path);
    if (res)
        goto out;

    for (;;) {
        res = f_readdir(&dir, &fno);
        if (res || !fno.fname[0])
            break;

        vecPushBack(files, fno);
    }

    f_closedir(&dir);
    sd_unmount();
    return files;
out:
    f_closedir(&dir);
    vecFree(files);
    sd_unmount();
    return NULL;
}

void pushsdMenu(void* data) {
    u32 i = 0;
    char* path = (char*)data;

    FILINFO* files = _listDirs(path);
    if (!files) return;

    menuEntry_t* entries = (menuEntry_t*)malloc(sizeof(menuEntry_t) * (vecGetCount(files)+3));
    if (!entries) goto out;

    entries[i++] = ENT_SEPARATOR();
    entries[i++] = ENT_BACK(COLOR_ORANGE, "<- Back");

    for (u32 j = 0; j < vecGetCount(files); i++, j++) {
                                // First %s      /                  Second %s
        char* fullPath = malloc(strlen(path) + (sizeof(char*)*1) + strlen(files[j].fname));
        s_printf(fullPath, "%s/%s", path, files[j].fname);

        entries[i] = files[j].fattrib & AM_DIR ?
        ENT_DIRECTORY(COLOR_GREEN, files[j].fname, fullPath, pushsdMenu) :
        ENT_FILE(     COLOR_BLUE, files[j].fname, fullPath, powerOff) ;
    }

    entries[i] = ENT_END();

    const menu_t sdMenu = {
        .title       = "SD Menu",
        .entries     = entries,
        .count       = i,
        .cursorIndex = 1,
        .x     = 0,    .y = 0,
        .w     = SCREEN_WIDTH,
        .h     = SCREEN_HEIGHT,

        .isOverlay   = false,
        .printHeader = true,
        .printFooter = true,
        .renderDirty = true,
        .titleStatic = true
    };

    pushMenu(sdMenu);

out:
    vecFree(files);
}