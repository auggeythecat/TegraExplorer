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
#include "../util/utils.h"
#include "../util/math.h"

// TODO: Decide on how I should format this.
static char fileSize[4] = {
    'B', // or "B",   or "  B", or " B", or something else
    'K', // or "KiB", or "KiB", or "KB", or something else
    'M', // or "MiB", or "MiB", or "MB", or something else
    'G', // or "GiB", or "GiB", or "GB", or something else
};

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

void pushsdMenu(char* path) {
    FILINFO* files = _listDirs(path);
    if (!files) return;

    const u32 fileCount = vecGetCount(files);

    menuEntry_t* entries = (menuEntry_t*)malloc((fileCount + 4) * sizeof(menuEntry_t));
    if (!entries) goto out;

    u32 i = 0;
    entries[i++] = ENT_SEPARATOR();
    entries[i++] = ENT_BACK(COLOR_ORANGE, "<- Back");
    entries[i++] = ENT_HANDLER(COLOR_LIGHT_GREY, "Special function button", powerOff);

    for (u32 j = 0; j < vecGetCount(files); i++, j++) {
        bool isDir = files[j].fattrib & AM_DIR;
        char fullPath[FF_MAX_LFN + 10]; // I think sd:/ wouldn't be counted, so I'm just going to add 10 to be safe.
        s_printf(fullPath, "%s/%s", path, files[j].fname);

        char caption[128];
        const u32 numLength = log10Approximation(files[j].fsize);

        s_printf(caption, isDir ? "%c|%s" : "%c|%s %d%c",
            isDir ? GFX_CHAR_FOLDER : GFX_CHAR_FILE,
            isDir ? path : path,
            isDir ? : (u32)(files[j].fsize / uintPow(10, numLength - 1)),
            isDir ? : fileSize[numLength / 3]
        );

        entries[i] = ENT_HANDLER_EX(
            isDir ? COLOR_GREEN : COLOR_BLUE,
            caption,
            isDir ? CAST_HANDLER(pushsdMenu) : CAST_HANDLER(powerOff), // TODO:
            fullPath
        );
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
        .renderDirty = true,
        .titleStatic = true,
        .fullHeight  = true
    };

    pushMenu(sdMenu);

out:
    vecFree(files);
}