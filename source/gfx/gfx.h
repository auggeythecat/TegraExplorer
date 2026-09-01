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

#ifndef _GFX_H
#define _GFX_H

#include <utils/types.h>
#include <memory_map.h>
#include <stdarg.h>

#include "colors.h"

#define     EPRINTF(text         ) gfx_printf("%k"text"%k\n", COLOR_RED,          COLOR_DEFAULT)
#define EPRINTFARGS(text, args...) gfx_printf("%k"text"%k\n", COLOR_RED,    args, COLOR_DEFAULT)

#define     WPRINTF(text         ) gfx_printf("%k"text"%k\n", COLOR_YELLOW,       COLOR_DEFAULT)
#define WPRINTFARGS(text, args...) gfx_printf("%k"text"%k\n", COLOR_YELLOW, args, COLOR_DEFAULT)

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define FONT_SIZE   8
#define PADDING     1
#define SDF_SIZE    (FONT_SIZE + (PADDING * 2))
#define INNER_SIZE  (SDF_SIZE  - (PADDING * 2))

#define MAX_ATLASES 16 + 1
#define NUM_CHARS   98

#define SDF_BUFFER  NYX_RES_ADDR

#define GFX_CHAR_SPACE    0
#define GFX_CHAR_FOLDER   127
#define GFX_CHAR_FILE     128
#define GFX_CHAR_CHARGING 129

typedef struct _sdfAtlas_t {
    u32 size;
    u8* data;
} sdfAtlas_t;

typedef struct _gfxCtxt_t {
    u32 *fb;
    u32 width;
    u32 height;
    u32 stride;
} gfxCtxt_t;

typedef struct _gfxCon_t {
    gfxCtxt_t *gfxCtxt;
    u32 fntsz;
    u32 x;
    u32 y;
    u32 savedx;
    u32 savedy;
    u32 fgcol;
    int fillbg;
    u32 bgcol;
    bool mute;
} gfxCon_t;

extern gfxCtxt_t gfxCtxt;
extern gfxCon_t  gfxCon;

extern gfxCtxt_t gfx_ctxt;
extern gfxCon_t  gfx_con;

void gfxRenderSDF();
void gfxBakeAtlas(u32 fontSize);
void gfxInitCtxt(u32 *fb, u32 width, u32 height, u32 stride);
void gfxConInit();
void gfxConSetCol(u32 fgcol, int fillbg, u32 bgcol);
void gfxConSetFontSize(u32 fontSize);
void gfxConGetPos(u32 *x, u32 *y);
void gfxConSetPos(u32 x, u32 y);
void __attribute__((target("arm"))) gfxPutC(char c);
void gfxPutS(const char *s);
void gfxPutSSmall(const char *s);
void gfxCPutS(u32 color, const char *s);
void gfxPutSLimit(const char *s, u32 limit);
void gfxVPrintF(const char *fmt, va_list ap);
void gfxPrintF(const char *fmt, ...); void gfx_printf(const char *fmt, ...);
void gfxSetPixel(u32 x, u32 y, u32 color);
void gfxLine(int x0, int y0, int x1, int y1, u32 color);
void gfxBoxGrey(u32 x0, u32 y0, u32 x1, u32 y1, u8 shade);
void gfxBoxARGB(u32 x0, u32 y0, u32 x1, u32 y1, u32 color);
void gfxClearGrey(u8 color);
void gfxClearPartialGrey(u8 color, u32 pos_y, u32 height);
void gfxClearColor(u32 color);

#endif //_GFX_H