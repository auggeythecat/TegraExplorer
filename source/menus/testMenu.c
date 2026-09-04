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
    ENT_SEPERATOR(                                                                  ),
      ENT_CAPTION(COLOR_WHITE ,  "---| BROWSE |---"                                 ),
      ENT_HANDLER(COLOR_GREEN , "Browse SD",                                powerOff),
      ENT_HANDLER(COLOR_GREEN , "Browse sysMMC",                            powerOff),
      ENT_HANDLER(COLOR_BLUE  , "Browse emuMMC",                            powerOff),
    ENT_SEPERATOR(                                                                  ),
      ENT_CAPTION(COLOR_WHITE ,  "---| EXIT |---"                                   ),
      ENT_HANDLER(COLOR_VIOLET, "Power off",                                powerOff),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to RCM",                           rebootRCM),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to OFW",                           rebootOFW),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to hekate",                         powerOff),
      ENT_HANDLER(COLOR_VIOLET, "Reboot to atmosphere",                     powerOff),
    ENT_SEPERATOR(                                                                  ),
      ENT_CAPTION(COLOR_WHITE , "---| TOOLS |---"                                   ),
      ENT_HANDLER(COLOR_GREEN , "Partition sd",                             powerOff),
      ENT_HANDLER(COLOR_GREEN , "View dumped keys",                         powerOff),
      ENT_HANDLER(COLOR_GREEN , "View credits",                             powerOff),
          ENT_END(                                                                  )

};

static menu_t testMenu = {
    .title   = "TegraExplorer",
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
