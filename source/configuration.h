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

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#define USE_VIC 1
#define COLOR_FG COLOR_WHITE
#define COLOR_BG COLOR_DARKER_GREY

#define BITMAP_FONT 1
#define SDF_FONT 1

#if SDF_FONT
#undef BITMAP_FONT
#define BITMAP_FONT 1

#define SDF_BUFFER NYX_RES_ADDR
#define SDF_PADDING 1
#endif

#endif //_CONFIGURATION_H