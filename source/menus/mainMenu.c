//
// Created by auggiethecat on 5/3/26.
//

#include "mainMenu.h"

#include <bdk.h>

#include "../gfx/menu.h"
#include "../gfx/gfx.h"
#include "../util/utils.h"


#include "explorers.h"

entry_t mainMenuEntries[] = {
     ENT_SEPERATOR(                                                                       ),
       ENT_CAPTION(COLOR_WHITE ,  "---| Browse |---"                                      ),
    ENT_HANDLER_EX(COLOR_GREEN , "Browse SD",                        "sd:", pushFolderMenu),
       ENT_HANDLER(COLOR_GREEN , "Browse SysMMC",                           rebootRCM     ),
       ENT_HANDLER(COLOR_BLUE  , "Browse EmuMMC",                           reboot        ),

       ENT_CAPTION(COLOR_WHITE ,  "---| Exit |---"                                        ),
       ENT_HANDLER(COLOR_VIOLET, "Power off",                               powerOff      ),
       ENT_HANDLER(COLOR_VIOLET, "Reboot to RCM",                           rebootRCM     ),
       ENT_HANDLER(COLOR_VIOLET, "Reboot to OFW",                           reboot        ),
       ENT_HANDLER(COLOR_VIOLET, "Reboot to bootloader/update.bin",         rebootHekate  ),
       ENT_HANDLER(COLOR_VIOLET, "Reboot to atmosphere/reboot_payload.bin", rebootAMS     ),
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
    .cursorIndex   = 2,

    .__static.entries = mainMenuEntries,
    .__static.count   = ARRAY_SIZE(mainMenuEntries)
};

void pushMainMenu() {
    pushMenu(mainMenu);
}