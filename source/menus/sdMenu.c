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
#include <string.h>
#include <utils/sprintf.h>

#include "../gfx/gfx.h"
#include "../gfx/menu.h"
#include "../util/utils.h"
#include "../util/vector.h"
#include "display/vic.h"
#include "soc/timer.h"
#include "storage/sd.h"

static vector_t _listDirs(const char* path) {
    int res = 0;
    DIR dir;
    FILINFO fno;

    FILINFO* files = newVec(FILINFO, 16);
    if (!files) return NULL;

    sd_mount();

    res = f_opendir(&dir, path);
    if (res) {
        gfxPrintF("%d", res);

        vic_compose();
        vic_wait_idle();

        usleep(5000000);

        goto out;
    }

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

    menuEntry_t* entries = (menuEntry_t*)malloc(sizeof(menuEntry_t)*(vecGetCount(files)+3));
    if (!entries) goto out;

    entries[i++] = (menuEntry_t)ENT_SEPERATOR();
    entries[i++] = (menuEntry_t)ENT_BACK(COLOR_ORANGE, "<- Back");

    for (u32 j = 0; j < vecGetCount(files); i++, j++) {
        char* fullPath = malloc(strlen(path) + strlen(files[j].fname));
        s_printf(fullPath, "%s/%s", path, files[j].fname);

        entries[i] = files[j].fattrib & AM_DIR ?
        (menuEntry_t) ENT_DIRECTORY(COLOR_VIOLET, files[j].fname, fullPath, pushsdMenu) :
        (menuEntry_t) ENT_FILE(     COLOR_VIOLET, files[j].fname, fullPath, pushsdMenu) ;
    }

    entries[i] = (menuEntry_t)ENT_END();

    const menu_t sdMenu = {
        .title   = "SD Menu",
        .entries = entries,
        .count   = i,
        .cursorIndex = 0,
        .x     = 0,    .y = 0,
        .w     = SCREEN_WIDTH,
        .h     = SCREEN_HEIGHT,

        .isOverlay   = false,
        .printHeader = true,
        .printFooter = true,
        .renderDirty = true,
    };

    pushMenu(sdMenu);

out:
    vecFree(files);
}