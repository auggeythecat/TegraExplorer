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

#include "error.h"

#include "../gfx/gfx.h"

#include <string.h>
#ifdef USE_VIC
#include "display/vic.h"
#endif
#include "hid.h"

#define  OUTER_LENGTH(fontSize) (36 * (fontSize))
#define  OUTER_HEIGHT(fontSize) (13 * (fontSize))
#define OUTER_START_X(fontSize) ((SCREEN_WIDTH  - OUTER_LENGTH(fontSize)) / 2)
#define OUTER_START_Y(fontSize) ((SCREEN_HEIGHT - OUTER_HEIGHT(fontSize)) / 2)
#define   OUTER_END_X(fontSize) (OUTER_START_X(fontSize) + OUTER_LENGTH(fontSize))
#define   OUTER_END_Y(fontSize) (OUTER_START_Y(fontSize) + OUTER_HEIGHT(fontSize))

#define INNER_START_X(fontSize)   (OUTER_START_X(fontSize) + fontSize*1)
#define INNER_START_Y(fontSize)   (OUTER_START_Y(fontSize) + fontSize*1)
#define   INNER_LENGTH(fontSize)  ( OUTER_LENGTH(fontSize) - fontSize*2)
#define   INNER_HEIGHT(fontSize)  ( OUTER_HEIGHT(fontSize) - fontSize*2)
#define    INNER_END_X(fontSize)  (INNER_START_X(fontSize) + INNER_LENGTH(fontSize))
#define    INNER_END_Y(fontSize)  (INNER_START_Y(fontSize) + INNER_HEIGHT(fontSize))

#define  TITLE_X(fontSize, length) (INNER_START_X(fontSize) + ((INNER_LENGTH(fontSize) - (fontSize * length)) / 2))
#define  TITLE_Y(fontSize)         (INNER_START_Y(fontSize) + fontSize*1)

#define   BODY_X(fontSize)         (INNER_START_X(fontSize) + fontSize*1)
#define   BODY_Y(fontSize)         (INNER_START_Y(fontSize) + fontSize*4)

#define FOOTER_X(fontSize, length) (INNER_START_X(fontSize) + ((INNER_LENGTH(fontSize) - (fontSize * length)) / 2))
#define FOOTER_Y(fontSize)         (  INNER_END_Y(fontSize) - fontSize*2)

static u32 _getLR() {
    u32 lr = 0;
    asm volatile("mov %[lr], lr\n\t" : [lr]"=&r"(lr) : :);
    return lr;
}

void drawError(const teError_t error) {
    if (error.code == 0) return;

    const u32 size = gfxCon.fntsz;

    gfxBoxARGB(OUTER_START_X(size), OUTER_START_Y(size), OUTER_END_X(size), OUTER_END_Y(size), error.accentColor);
    gfxBoxARGB(INNER_START_X(size), INNER_START_Y(size), INNER_END_X(size), INNER_END_Y(size), COLOR_BG);

    SETCOLOR(error.accentColor, COLOR_BG);

    const char* titleString = "An error occurred!";
    gfxConSetPos(TITLE_X(size, strlen(titleString)), TITLE_Y(size));
    gfxPutS(titleString);

    gfxConSetPos(BODY_X(size), BODY_Y(size));
    gfxPrintF("%NModule: %s (%d)%nCode  : %d%nAddr  : %x", gfxCon.x, error.moduleName, error.module, error.code, _getLR());

    const char* footerString = "Press A to continue.";
    gfxConSetPos(FOOTER_X(size, strlen(footerString)), FOOTER_Y(size));
    gfxPutS(footerString);

#ifdef USE_VIC
    vic_compose();
    vic_wait_idle();
#endif

    hidWaitMask(JOYA | JOYB);
}