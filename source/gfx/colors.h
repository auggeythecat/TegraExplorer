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

#ifndef _COLORS_H
#define _COLORS_H

#define COLOR_TRANSPARENT  0x00000000
#define COLOR_NONE         COLOR_TRANSPARENT

#define COLOR_RED          0xFFE70000
#define COLOR_ORANGE       0xFFFF8C00
#define COLOR_YELLOW       0xFFFFFF40
#define COLOR_GREEN        0xFF40FF00
#define COLOR_BLUE         0xFF00DDFF
#define COLOR_VIOLET       0xFF8040FF

#define COLOR_WHITEST      0xFFFFFFFF
#define COLOR_WHITE        0xFFF0F0F0
#define COLOR_LIGHTER_GREY 0xFFD0D0D0
#define COLOR_LIGHT_GREY   0xFFA0A0A0
#define COLOR_GREY         0xFF808080
#define COLOR_DARK_GREY    0xFF606060
#define COLOR_DARKER_GREY  0xFF303030
#define COLOR_BLACK        0xFF000000

#define COLOR_DEFAULT      0xFF1B1B1B

#define RGBTOCOLOR(r, g, b) (b << 16 | g << 8 | r << 0)
#define GREYTOCOLOR(s)      (s << 16 | s << 8 | s << 0)
#define COLORTORGB(color)   (color & 0x00FFFFFF)
#define INVERTCOLOR(color)  (~(color & 0x00FFFFFF))
#define SETCOLOR(fg, bg)    gfxConSetCol(fg, 1, bg)
#define RESETCOLOR          SETCOLOR(COLOR_WHITE, COLOR_DEFAULT);

#define FILLBG   1
#define NOFILLBG 0

#endif //_COLORS_H