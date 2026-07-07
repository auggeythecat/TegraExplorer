//
// Created by auggiethecat on 5/3/26.
//

#include <bdk.h>

#include "../gfx/menu.h"
#include "../gfx/gfx.h"
#include "../util/utils.h"

u32 getCount(void* stuff) {
    return vecGetCount(stuff);
}

menu_t SDMenu = {
    .title = "Browse SD",
    .x     = 0,    .y = 0,
    .w     = SCREEN_WIDTH,
    .h     = SCREEN_HEIGHT,
    .idx   = 0,    .offset = 0,

    .isOverlay     = false,
    .isDynamic     = true ,
    .isPageCount   = true ,
    .isFileBrowser = true ,
    .cursorIndex   = 3    ,

    .__dynamic.data        = "sd:/",
    .__dynamic.get_count   = getCount,
    .__dynamic.get_entries = NULL,
};
entry_t mainMenuEntries[] = {
    ENT_SEPERATOR(                                                                     ),
      ENT_CAPTION(COLOR_WHITE ,  "---| Browse |---"                                    ),
         ENT_MENU(COLOR_GREEN , "Browse SD",                               &SDMenu     ),
      ENT_HANDLER(COLOR_GREEN , "Browse SysMMC",                           rebootRCM   ),
      ENT_HANDLER(COLOR_BLUE  , "Browse EmuMMC",                           reboot      ),

      ENT_CAPTION(COLOR_WHITE ,  "---| Exit |---"                                      ),
      ENT_HANDLER(COLOR_VIOLET, "Power off",                               powerOff    ),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to RCM",                           rebootRCM   ),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to OFW",                           reboot      ),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to bootloader/update.bin",         rebootHekate),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to atmosphere/reboot_payload.bin", rebootAMS   ),
    // MainTools,
    // MainPartitionSd,
    // MainViewKeys,
    // MainViewCredits,
    ENT_END()

};

menu_t mainMenu = {
    .title = "TegraExplorer",
    .x     = 0,    .y = 0,
    .w     = SCREEN_WIDTH,
    .h     = SCREEN_HEIGHT,
    .idx   = 0,    .offset = 0,

    .isOverlay     = false,
    .isDynamic     = false,
    .isPageCount   = true ,
    .isFileBrowser = false,
    .cursorIndex   = 3,

    .__static.entries = mainMenuEntries,
    .__static.count   = ARRAY_SIZE(mainMenuEntries)
};

void pushMainMenu() {
    pushMenu(mainMenu);
}