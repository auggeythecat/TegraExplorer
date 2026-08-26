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

#include "gfx.h"

#include <stdarg.h>

#include "../util/fixedpoint.h"
#include "../util/math.h"

#include <utils/util.h>
#include <string.h>

// Lifted from https://github.com/epto/epto-fonts/ directly
// I just love the capital letters especially from figo1
static const u8 _font[NUM_CHARS][FONT_SIZE] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // char 032 ( )
    {0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00}, // char 033 (!)
    {0x6C, 0x6C, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00}, // char 034 (")
    {0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00}, // char 035 (#)
    {0x10, 0x7E, 0xD0, 0x7C, 0x16, 0xFC, 0x10, 0x00}, // char 036 ($)
    {0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00}, // char 037 (%)
    {0x38, 0x6C, 0x68, 0x76, 0xDC, 0xCC, 0x76, 0x00}, // char 038 (&)
    {0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00}, // char 039 (')
    {0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00}, // char 040 (()
    {0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00}, // char 041 ())
    {0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00}, // char 042 (*)
    {0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00}, // char 043 (+)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30}, // char 044 (,)
    {0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00}, // char 045 (-)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00}, // char 046 (.)
    {0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00}, // char 047 (/)
    {0xFC, 0x06, 0x6E, 0x7E, 0x76, 0x66, 0x3C, 0x00}, // char 048 (0)
    {0x38, 0x00, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00}, // char 049 (1)
    {0x7C, 0x60, 0x06, 0x3C, 0x60, 0x66, 0x7E, 0x00}, // char 050 (2)
    {0x7E, 0x60, 0x06, 0x1C, 0x06, 0x66, 0x7C, 0x00}, // char 051 (3)
    {0xCE, 0xC0, 0xCC, 0xFE, 0x0C, 0x0C, 0x1E, 0x00}, // char 052 (4)
    {0xFE, 0x06, 0x60, 0x7C, 0x06, 0x66, 0x7C, 0x00}, // char 053 (5)
    {0xFE, 0x06, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00}, // char 054 (6)
    {0x7E, 0x60, 0x06, 0x1C, 0x18, 0x18, 0x18, 0x00}, // char 055 (7)
    {0xFC, 0x06, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00}, // char 056 (8)
    {0xFC, 0x06, 0x66, 0x3E, 0x06, 0x66, 0x7C, 0x00}, // char 057 (9)
    {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00}, // char 058 (:)
    {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30}, // char 059 (;)
    {0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00}, // char 060 (<)
    {0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00}, // char 061 (=)
    {0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00}, // char 062 (>)
    {0x7C, 0x66, 0x06, 0x0C, 0x18, 0x00, 0x18, 0x00}, // char 063 (?)
    {0xFC, 0x06, 0xC6, 0xDE, 0xDE, 0xC0, 0x7E, 0x00}, // char 064 (@)
    {0xFC, 0x06, 0x66, 0x7E, 0x66, 0x66, 0xE6, 0x00}, // char 065 (A)
    {0xFC, 0x06, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00}, // char 066 (B)
    {0xFE, 0x06, 0x60, 0x60, 0x60, 0x66, 0x3E, 0x00}, // char 067 (C)
    {0xFC, 0x06, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00}, // char 068 (D)
    {0xFE, 0x06, 0x60, 0x78, 0x60, 0x66, 0xFE, 0x00}, // char 069 (E)
    {0xFE, 0x06, 0x60, 0x78, 0x60, 0x60, 0xF0, 0x00}, // char 070 (F)
    {0xFE, 0x06, 0x60, 0x6E, 0x66, 0x66, 0x3E, 0x00}, // char 071 (G)
    {0xE6, 0x06, 0x66, 0x7E, 0x66, 0x66, 0xE6, 0x00}, // char 072 (H)
    {0x7E, 0x00, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00}, // char 073 (I)
    {0x3E, 0x30, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00}, // char 074 (J)
    {0xEC, 0x0C, 0x6C, 0x7C, 0x66, 0x66, 0xE6, 0x00}, // char 075 (K)
    {0xF0, 0x00, 0x60, 0x60, 0x60, 0x66, 0xFE, 0x00}, // char 076 (L)
    {0xE3, 0x07, 0x7F, 0x6B, 0x63, 0x63, 0xE3, 0x00}, // char 077 (M)
    {0xE6, 0x06, 0x76, 0x7E, 0x6E, 0x66, 0xE6, 0x00}, // char 078 (O)
    {0xFC, 0x06, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}, // char 079 (N)
    {0xFC, 0x06, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00}, // char 080 (P)
    {0xFC, 0x06, 0x66, 0x66, 0x6E, 0x6E, 0x3E, 0x00}, // char 081 (Q)
    {0xFC, 0x06, 0x66, 0x7C, 0x66, 0x66, 0xE6, 0x00}, // char 082 (R)
    {0xFE, 0x06, 0xC0, 0x7C, 0x06, 0xC6, 0xFC, 0x00}, // char 083 (S)
    {0xFE, 0x02, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00}, // char 084 (T)
    {0xE6, 0x06, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00}, // char 085 (U)
    {0xE6, 0x06, 0x66, 0x66, 0x6C, 0x7C, 0xF8, 0x00}, // char 086 (V)
    {0xE3, 0x03, 0x63, 0x6B, 0x7F, 0x77, 0xE3, 0x00}, // char 087 (W)
    {0xE6, 0x06, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00}, // char 088 (X)
    {0xE6, 0x06, 0x66, 0x3E, 0x06, 0x66, 0x7C, 0x00}, // char 089 (Y)
    {0x7E, 0x60, 0x06, 0x3C, 0x60, 0x66, 0x7E, 0x00}, // char 090 (Z)
    {0x3C, 0x00, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00}, // char 091 ([)
    {0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x00}, // char 092 (\)
    {0x3C, 0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00}, // char 093 (])
    {0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00}, // char 094 (^)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE}, // char 095 (_)
    {0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00}, // char 096 (`)
    {0x00, 0x00, 0x7C, 0x06, 0x3E, 0x76, 0x3E, 0x00}, // char 097 (a)
    {0xE0, 0x40, 0x7C, 0x6E, 0x66, 0x66, 0xFC, 0x00}, // char 098 (b)
    {0x00, 0x00, 0x7C, 0x36, 0x60, 0x66, 0x3C, 0x00}, // char 099 (c)
    {0x07, 0x02, 0x3E, 0x76, 0x66, 0x66, 0x3F, 0x00}, // char 100 (d)
    {0x00, 0x00, 0x7C, 0x36, 0x7E, 0x60, 0x3E, 0x00}, // char 101 (e)
    {0x3C, 0x26, 0x30, 0x78, 0x30, 0x30, 0x78, 0x00}, // char 102 (f)
    {0x00, 0x00, 0x7E, 0xEC, 0xCC, 0x7C, 0x0C, 0xF8}, // char 103 (g)
    {0xE0, 0x40, 0x7C, 0x6E, 0x66, 0x66, 0xE6, 0x00}, // char 104 (h)
    {0x38, 0x00, 0x30, 0x18, 0x18, 0x18, 0x3C, 0x00}, // char 105 (i)
    {0x1C, 0x00, 0x18, 0x0C, 0x0C, 0x0C, 0xCC, 0x78}, // char 106 (j)
    {0xE0, 0x40, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00}, // char 107 (k)
    {0x38, 0x10, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00}, // char 108 (l)
    {0x00, 0x00, 0xCC, 0xFE, 0xDE, 0xC6, 0xC6, 0x00}, // char 109 (m)
    {0x00, 0x00, 0x7C, 0x6E, 0x66, 0x66, 0x66, 0x00}, // char 110 (o)
    {0x00, 0x00, 0x7C, 0x2E, 0x66, 0x66, 0x3C, 0x00}, // char 111 (n)
    {0x00, 0x00, 0xFC, 0x6E, 0x66, 0x7C, 0x40, 0xE0}, // char 112 (p)
    {0x00, 0x00, 0x7E, 0xEC, 0xCC, 0x7C, 0x04, 0x0E}, // char 113 (q)
    {0x00, 0x00, 0xFC, 0x6E, 0x60, 0x60, 0xF0, 0x00}, // char 114 (r)
    {0x00, 0x00, 0x3E, 0x70, 0x3C, 0x06, 0x7C, 0x00}, // char 115 (s)
    {0x38, 0x10, 0x7C, 0x30, 0x30, 0x34, 0x18, 0x00}, // char 116 (t)
    {0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x7E, 0x00}, // char 117 (u)
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00}, // char 118 (v)
    {0x00, 0x00, 0xC6, 0xC6, 0xD6, 0x7C, 0x6C, 0x00}, // char 119 (w)
    {0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00}, // char 120 (x)
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x70}, // char 121 (y)
    {0x00, 0x00, 0x7E, 0x6C, 0x18, 0x32, 0x7E, 0x00}, // char 122 (z)
    {0x0E, 0x00, 0x18, 0x70, 0x18, 0x18, 0x0E, 0x00}, // char 123 ({)
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, // char 124 (|)
    {0x70, 0x00, 0x18, 0x0E, 0x18, 0x18, 0x70, 0x00}, // char 125 (})
    {0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // char 126 (~)
    {0x00, 0x0C, 0x12, 0x7E, 0x42, 0x42, 0x7E, 0x00}, // 127 (folder)
    {0x00, 0x0E, 0x12, 0x22, 0x22, 0x22, 0x3E, 0x00}, // 128   (file)
    {0x00, 0x08, 0x0C, 0x0E, 0x7E, 0x70, 0x30, 0x10}, // 129 (Charge)
};

void gfxRenderSDF() {
	for (int i = 0; i < NUM_CHARS; i++) {
		for (int y = 0; y < SDF_SIZE; y++) {
			const int vy = ((y - PADDING) * FONT_SIZE / INNER_SIZE);

			for (int x = 0; x < SDF_SIZE; x++) {
				const int vx = ((x - PADDING) * FONT_SIZE / INNER_SIZE);

				bool inside = false;

				if (vx >= 0 && vx < FONT_SIZE && vy >= 0 && vy < FONT_SIZE) {
					inside = (_font[FONT_SIZE*i][vy] >> vx) & 1;
				}

				u32 minDistance = 1000000;

				for (int sy = 0; sy < SDF_SIZE; sy++) {
					const int svy = ((sy - PADDING) * FONT_SIZE / INNER_SIZE);

					for (int sx = 0; sx < SDF_SIZE; sx++) {
						const int svx = ((sx - PADDING) * FONT_SIZE / INNER_SIZE);

						bool sinside = false;

						if (svx >= 0 && svx < FONT_SIZE && svy >= 0 && svy < FONT_SIZE)
							sinside = (_font[FONT_SIZE*i][svy] >> svx) & 1;

						if (sinside != inside) {
							const s32 dx     = x - sx;
							const s32 dy     = y - sy;
							const u64 distSq = dx*dx + dy*dy;
							if (distSq < minDistance) minDistance = distSq;
						}
					}
				}
				const u64 dist   = 1 + sqrt64(minDistance * 256);
				const u32 index  = SDF_SIZE*SDF_SIZE*i + SDF_SIZE*y + x;
				const int offset = dist - 8;
				((u8*)SDF_BUFFER)[index] = inside ? CLAMPMAX(128 + offset, 255) : CLAMPMIN(128 - offset, 0);
			}
		}
	}
}

static sdfAtlas_t atlases[MAX_ATLASES];
static u32 atlasCount   = 0;
static u8* atlasPointer = (u8*) SDF_BUFFER + SZ_32K;

static u8* _gfxGetAtlas(const u32 size) {
	for (u32 i = 0; i < atlasCount; i++) {
		if (atlases[i].size == size) return atlases[i].data;
	}

	if (atlasCount < MAX_ATLASES) {
	    // Will I ever find a good name for this variable?
		const u32 muhBytes  = NUM_CHARS * (size*size);

		u8* startPtr  = (u8*)atlasPointer;
		atlasPointer += (muhBytes + 3) & ~3;

		atlases[atlasCount++] = (sdfAtlas_t) {size, startPtr};
		return startPtr;
	}
	return NULL;
}

void gfxBakeAtlas(const u32 fontSize) {
	u8 *atlasBuf = _gfxGetAtlas(fontSize);
	if (!atlasBuf) return;

	// Fine-tuned number
	const sfp16_t scaleFactor = SFP16FROMINT(1) + (SFP16FROMINT(24) / 100);

	for (int i = 0; i < NUM_CHARS; i++) {
		const u8* charSDFBase = &((u8*)SDF_BUFFER)[i * (SDF_SIZE * SDF_SIZE)];

		for (u32 py = 0; py < fontSize; py++) {
			const sfp16_t normY       = sfp16Div(SFP16FROMINT(py), SFP16FROMINT(fontSize - 1));
			const sfp16_t scaledNormY = SFP16HALF + sfp16Mul(normY - SFP16HALF, sfp16Div(SFP16FROMINT(1), scaleFactor));
			const sfp16_t yPos        = sfp16Mul(scaledNormY, SFP16FROMINT(SDF_SIZE - 1));

			for (u32 px = 0; px < fontSize; px++) {
				const sfp16_t normX       = sfp16Div(SFP16FROMINT(px), SFP16FROMINT(fontSize - 1));
				const sfp16_t scaledNormX = SFP16HALF + sfp16Mul(normX - SFP16HALF, sfp16Div(SFP16FROMINT(1), scaleFactor));
				const sfp16_t xPos        = sfp16Mul(scaledNormX, SFP16FROMINT(SDF_SIZE - 1));

				const int ix = SFP16GETINT(xPos);
				const int iy = SFP16GETINT(yPos);
				const int fx = SFP16GETDEC(xPos);
				const int fy = SFP16GETDEC(yPos);

				int cix = ix + 1;
				int ciy = iy + 1;
				if (ciy >= SDF_SIZE) ciy = SDF_SIZE - 1;
				if (cix >= SDF_SIZE) cix = SDF_SIZE - 1;

				const int s00 = charSDFBase[ iy * SDF_SIZE +  ix];
				const int s10 = charSDFBase[ iy * SDF_SIZE + cix];
				const int s01 = charSDFBase[ciy * SDF_SIZE +  ix];
				const int s11 = charSDFBase[ciy * SDF_SIZE + cix];

				const int top  = s00 + (((s10 - s00) * fx) >> 16);
				const int bot  = s01 + (((s11 - s01) * fx) >> 16);
				const int sam  = top + (((bot - top) * fy) >> 16);

				const u32 index = i * (fontSize * fontSize) + (py * fontSize) + px;

				const int alpha = CLAMP((sam - 128) * 255, 0, 255);

				atlasBuf[index] = (u8)alpha;
			}
		}
	}
}

gfxCtxt_t gfxCtxt;
gfxCon_t gfxCon;

extern gfxCtxt_t gfx_ctxt __attribute__((alias("gfxCtxt")));
extern gfxCon_t  gfx_con  __attribute__((alias("gfxCon")));

static bool gfxConInitDone = false;

void gfxInitCtxt(u32 *fb, const u32 width, const u32 height, const u32 stride) {
	gfxCtxt.fb     = fb;
	gfxCtxt.width  = width;
	gfxCtxt.height = height;
	gfxCtxt.stride = stride;
}

// gfx_con is required by bdk, so I'm just going to copy-paste it.
void gfxConInit() {
	gfxCon.gfxCtxt   = &gfxCtxt;
	gfxCon.fntsz     = 16;
	gfxCon.x         = 0;
	gfxCon.y         = 0;
	gfxCon.savedx    = 0;
	gfxCon.savedy    = 0;
	gfxCon.fgcol     = COLOR_GREEN;
	gfxCon.fillbg    = 1;
	gfxCon.bgcol     = COLOR_DEFAULT;
	gfxCon.mute      = 0;

	gfxConInitDone   = true;

	gfxRenderSDF();
	gfxBakeAtlas(gfxCon.fntsz);
}

void gfxConSetCol(const u32 fgcol, const int fillbg, const u32 bgcol) {
    gfxCon.fgcol  = fgcol;
    gfxCon.fillbg = fillbg;
    gfxCon.bgcol  = bgcol;
}

void gfxConSetFontSize(const u32 fontSize) {
    gfxCon.fntsz = fontSize;
    gfxBakeAtlas(fontSize);
}

void gfxConGetPos(u32 *x, u32 *y) {
    *y = gfxCon.y;
    *x = gfxCon.x;
}

void gfxConSetPos(const u32 x, const u32 y) {
    gfxCon.y = y;
    gfxCon.x = x;
}

void __attribute__((target("arm"))) gfxPutc(const char c) {
    if unlikely(c <= 31 || c >= 129) {
        if (c == '\n') {
            gfxCon.x = 0;
            gfxCon.y += gfxCon.fntsz;
            if (gfxCon.y > gfxCtxt.height - gfxCon.fntsz)
                gfxCon.y = 0;
        }
        return;
    }
    const u32 sz = gfxCon.fntsz;

    u8 * restrict data = &_gfxGetAtlas(sz)[(c - 32) * (sz * sz)];
    u32* restrict fb   = gfxCtxt.fb + gfxCon.x + (gfxCon.y * gfxCtxt.stride);

    const u32 mask  = 0x00FF00FF;

    const u32 fg    = gfxCon.fgcol;
    const u32 fg_rb =  fg       & mask;
    const u32 fg_ag = (fg >> 8) & mask;

    const u32 bg    = gfxCon.bgcol;
    const u32 bg_rb =  bg       & mask;
    const u32 bg_ag = (bg >> 8) & mask;

    for (u32 y = 0; y < sz; y++) {
        for (u32 x = 0; x < sz; x++) {
            const u8 alpha = *data++;

                 if (alpha == 0)   { *fb++ = bg; }
            else if (alpha == 255) { *fb++ = fg; }
            else {
                const u32 rb = ((bg_rb + (((fg_rb - bg_rb) * alpha) >> 8)) & mask)     ;
                const u32 ag = ((bg_ag + (((fg_ag - bg_ag) * alpha) >> 8)) & mask) << 8;

                *fb++ = rb | ag;
            }
        }
        fb += gfxCtxt.stride - sz;
    }
    gfxCon.x += sz;
}

void gfxPuts(const char *s) {
    if (!s || !gfxConInitDone || gfxCon.mute)
        return;

    for (; *s; s++)
        gfxPutc(*s);
}

void gfxPutsSmall(const char *s) {
    if (!s || !gfxConInitDone || gfxCon.mute)
        return;

    gfxConSetFontSize(8);

    for (; *s; s++)
        gfxPutc(*s);
}

void gfxCPuts(const u32 color, const char *s) {
    gfxCon.fgcol = color;
    gfxPuts(s);
    gfxPutc('\n');
    gfxCon.fgcol = COLOR_DEFAULT;
}

void gfxPutsLimit(const char *s, const u32 limit){
    if (!s || gfxCon.mute)
        return;

    const u32 len = strlen(s);
    u32 charLimit = limit / gfxCon.fntsz;

    if (charLimit > len) {
        gfxPuts(s);
        return;
    }

    if (len > charLimit)
        charLimit -= 4;

    for (u32 i = 0; i < MIN(len, charLimit); i++)
        gfxPutc(s[i]);

    if (len > charLimit + 4)
        gfxPuts("... ");
}

static void _gfx_putn(u32 v, const int base, const char fill, const int fcnt) {
    static const char digits[] = "0123456789ABCDEF";

    char buf[65];
    int c = fcnt;
    bool negative = false;

    if (base != 10 && base != 16)
        return;

    // Account for negative numbers.
    if (base == 10 && v & 0x80000000) {
        negative = true;
        v = (int)v * -1;
        c--;
    }

    char *p = buf + 64;
    *p = 0;
    do {
        c--;
        *--p = digits[v % base];
        v /= base;
    } while (v);

    if (negative)
        *--p = '-';

    if (fill != 0) {
        while (c > 0 && p > buf) {
            *--p = fill;
            c--;
        }
    }

    gfxPuts(p);
}

void gfxVPrintF(const char *fmt, va_list ap) {
    if (!gfxConInitDone || gfxCon.mute)
        return;

    u32 newLineConfig = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            int fill = 0;
            int fcnt = 0;
            if ((*fmt >= '0' && *fmt <= '9') || *fmt == ' ') {
                fcnt = *fmt;
                fmt++;
                if (*fmt >= '0' && *fmt <= '9') {
                    fill = fcnt;
                    fcnt = *fmt - '0';
                    fmt++;
                } else {
                    fill = ' ';
                    fcnt -= '0';
                }
            } switch(*fmt) {
            case 'c':
                gfxPutc(va_arg(ap, u32));
                break;
            case 's':
                gfxPuts(va_arg(ap, char *));
                break;
            case 'd':
                _gfx_putn(va_arg(ap, u32), 10, fill, fcnt);
                break;
            case 'n':
                gfxPutc('\n');
                gfxCon.x = newLineConfig;
                break;
            case 'N':
                newLineConfig = va_arg(ap, u32);
                gfxCon.x = newLineConfig;
                break;
            case 'p':
            case 'P':
            case 'x':
            case 'X':
                _gfx_putn(va_arg(ap, u32), 16, fill, fcnt);
                break;
            case 'k':
                gfxCon.fgcol = va_arg(ap, u32);
                break;
            case 'K':
                gfxCon.bgcol = va_arg(ap, u32);
                gfxCon.fillbg = 1;
                break;
            case '%':
                gfxPutc('%');
                break;
            case '\0':
                return;
            default:
                gfxPutc('%');
                gfxPutc(*fmt);
                break;
            }
        } else
            gfxPutc(*fmt);
        fmt++;
    }
}

void gfxPrintF(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    gfxVPrintF(fmt, ap);
    va_end(ap);
}

__attribute__((alias("gfxPrintF"))) void gfx_printf(const char *fmt, ...);