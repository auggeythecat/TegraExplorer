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


#include "testMenu.h"

#include "../gfx/menu.h"
#include "../gfx/gfx.h"
#include "../util/utils.h"

static menuEntry_t testMenuEntries[] = {
    ENT_SEPERATOR(                                                                       ),
      ENT_CAPTION(COLOR_WHITE ,  "---| BROWSE |---"                                      ),
   ENT_HANDLER_EX(COLOR_GREEN , "BROWSE SD",                        "sd:", powerOff      ),
      ENT_HANDLER(COLOR_GREEN , "BROWSE SYSMMC",                           rebootRCM     ),
      ENT_HANDLER(COLOR_BLUE  , "BROWSE EMUMMC",                           rebootOFW     ),

      ENT_CAPTION(COLOR_WHITE ,  "---| EXIT |---"                                        ),
      ENT_HANDLER(COLOR_VIOLET, "POWER OFF",                               powerOff      ),
      ENT_HANDLER(COLOR_VIOLET, "REBOOT TO RCM",                           rebootRCM     ),
      ENT_HANDLER(COLOR_VIOLET, "REBOOT TO OFW",                           rebootOFW     ),
//      ENT_HANDLER(COLOR_VIOLET, "REBOOT TO bootloader/update.bin",         powerOff      ),
//      ENT_HANDLER(COLOR_VIOLET, "REBOOT TO atmosphere/reboot_payload.bin", powerOff      ),
          ENT_END(                                                                       )
   // MainTools,
   // MainPartitionSd,
   // MainViewKeys,
   // MainViewCredits,

};

static menu_t testMenu = {
    .title = "TegraExplorer",
    .entries = testMenuEntries,
    .count   = ARRAY_SIZE(testMenuEntries),
    .cursorIndex = 0,
    .x     = 0,    .y = 0,
    .w     = SCREEN_WIDTH,
    .h     = SCREEN_HEIGHT,

    .isOverlay   = false,
    .printHeader = true,
    .printFooter = true,
    .renderDirty = true,
};

void pushTestMenu() {
    pushMenu(testMenu);
}
