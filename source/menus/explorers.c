//
// Created by auggiethecat on 7/9/26.
//

#include "explorers.h"

#include <bdk.h>

#include "../gfx/menu.h"


void pushFolderMenu(void *path) {
    menu_t folderMenu = {
        .title = "Browse SD",
        .x     = 0,    .y = 0,
        .w     = SCREEN_WIDTH,
        .h     = SCREEN_HEIGHT,
        .idx   = 0,    .offset = 0,

        .isOverlay     = false,
        .isDynamic     = true ,
        .isPageCount   = true ,
        .isFileBrowser = true ,
        .cursorIndex   = 2    ,

        .__dynamic.path    = path,
        .__dynamic.entries = NULL
    };

    pushMenu(folderMenu);
}