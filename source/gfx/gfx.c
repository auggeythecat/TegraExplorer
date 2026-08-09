/*
* Copyright (c) 2018 naehrwert
 * Copyright (c) 2018-2022 CTCaer
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <string.h>
#include "gfx.h"
#include "../util/fixedpoint.h"

gfxCtxt_t gfx_ctxt;
gfxCon_t gfx_con;

static bool gfx_con_init_done = false;

static const u8 _gfx_font[NUM_CHARS * NATIVE_FONT_SIZE] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0020 ( )
    0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00,   // U+0021 (!)
    0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0022 (")
    0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00,   // U+0023 (#)
    0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00,   // U+0024 ($)
    0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00,   // U+0025 (%)
    0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00,   // U+0026 (&)
    0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0027 (')
    0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00,   // U+0028 (()
    0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00,   // U+0029 ())
    0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00,   // U+002A (*)
    0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00,   // U+002B (+)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06,   // U+002C (,)
    0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00,   // U+002D (-)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00,   // U+002E (.)
    0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00,   // U+002F (/)
    0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00,   // U+0030 (0)
    0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00,   // U+0031 (1)
    0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00,   // U+0032 (2)
    0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00,   // U+0033 (3)
    0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00,   // U+0034 (4)
    0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00,   // U+0035 (5)
    0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00,   // U+0036 (6)
    0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00,   // U+0037 (7)
    0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00,   // U+0038 (8)
    0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00,   // U+0039 (9)
    0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00,   // U+003A (:)
    0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06,   // U+003B (;)
    0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00,   // U+003C (<)
    0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00,   // U+003D (=)
    0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00,   // U+003E (>)
    0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00,   // U+003F (?)
    0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00,   // U+0040 (@)
    0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00,   // U+0041 (A)
    0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00,   // U+0042 (B)
    0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00,   // U+0043 (C)
    0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00,   // U+0044 (D)
    0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00,   // U+0045 (E)
    0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00,   // U+0046 (F)
    0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00,   // U+0047 (G)
    0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00,   // U+0048 (H)
    0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+0049 (I)
    0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00,   // U+004A (J)
    0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00,   // U+004B (K)
    0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00,   // U+004C (L)
    0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00,   // U+004D (M)
    0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00,   // U+004E (N)
    0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00,   // U+004F (O)
    0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00,   // U+0050 (P)
    0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00,   // U+0051 (Q)
    0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00,   // U+0052 (R)
    0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00,   // U+0053 (S)
    0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+0054 (T)
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00,   // U+0055 (U)
    0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00,   // U+0056 (V)
    0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00,   // U+0057 (W)
    0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00,   // U+0058 (X)
    0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00,   // U+0059 (Y)
    0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00,   // U+005A (Z)
    0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00,   // U+005B ([)
    0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00,   // U+005C (\)
    0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00,   // U+005D (])
    0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00,   // U+005E (^)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,   // U+005F (_)
    0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0060 (`)
    0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00,   // U+0061 (a)
    0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00,   // U+0062 (b)
    0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00,   // U+0063 (c)
    0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00,   // U+0064 (d)
    0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00,   // U+0065 (e)
    0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00,   // U+0066 (f)
    0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F,   // U+0067 (g)
    0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00,   // U+0068 (h)
    0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+0069 (i)
    0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E,   // U+006A (j)
    0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00,   // U+006B (k)
    0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+006C (l)
    0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00,   // U+006D (m)
    0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00,   // U+006E (n)
    0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00,   // U+006F (o)
    0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F,   // U+0070 (p)
    0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78,   // U+0071 (q)
    0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00,   // U+0072 (r)
    0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00,   // U+0073 (s)
    0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00,   // U+0074 (t)
    0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00,   // U+0075 (u)
    0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00,   // U+0076 (v)
    0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00,   // U+0077 (w)
    0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00,   // U+0078 (x)
    0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F,   // U+0079 (y)
    0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00,   // U+007A (z)
    0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00,   // U+007B ({)
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,   // U+007C (|)
    0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00,   // U+007D (})
    0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+007E (~)
	0x00, 0x0C, 0x12, 0x7E, 0x42, 0x42, 0x7E, 0x00,   //   (folder)
	0x00, 0x0E, 0x12, 0x22, 0x22, 0x22, 0x3E, 0x00,   //     (file)
	0x00, 0x08, 0x0C, 0x0E, 0x7E, 0x70, 0x30, 0x10,   // (Charging)
};

void gfxRenderSDF() {
	for (int i = 0; i < ARRAY_SIZE(_gfx_font) / NATIVE_FONT_SIZE; i++) {
		for (int y = 0; y < SDF_SIZE; y++) {
			int vy = ((y - PADDING) * NATIVE_FONT_SIZE / INNER_SIZE);

			for (int x = 0; x < SDF_SIZE; x++) {
				int vx = ((x - PADDING) * NATIVE_FONT_SIZE / INNER_SIZE);

				bool inside = false;

				if (vx >= 0 && vx < NATIVE_FONT_SIZE && vy >= 0 && vy < NATIVE_FONT_SIZE) {
					inside = (_gfx_font[NATIVE_FONT_SIZE * i + vy] >> vx) & 1;
				}

				u32 minDistance = 1000000;

				for (int sy = 0; sy < SDF_SIZE; sy++) {
					int svy = ((sy - PADDING) * NATIVE_FONT_SIZE / INNER_SIZE);
					for (int sx = 0; sx < SDF_SIZE; sx++) {
						int svx = ((sx - PADDING) * NATIVE_FONT_SIZE / INNER_SIZE);

						bool sinside = false;

						if (svx >= 0 && svx < NATIVE_FONT_SIZE && svy >= 0 && svy < NATIVE_FONT_SIZE) {
							sinside = (_gfx_font[NATIVE_FONT_SIZE*i + svy] >> svx) & 1;
						}

						if (sinside != inside) {
							s32 dx = x - sx;
							s32 dy = y - sy;
							u64 distSq = dx*dx + dy*dy;
							if (distSq < minDistance) minDistance = distSq;
						}
					}
				}
				u64 dist = 1 + sqrt64(minDistance * 256);
				u32 index = SDF_SIZE*SDF_SIZE*i + SDF_SIZE*y + x;
				int offset = dist - 8;
				((u8*)SDF_BUFFER)[index] = inside ? CLAMPMAX(128 + offset, 255) : CLAMPMIN(128 - offset, 0);
			}
		}
	}
}

static sdfAtlas_t atlases[MAX_ATLASES];
static u32 atlas_count   = 0;
static u8* atlas_pointer = (u8*) SDF_BUFFER + SZ_32K;

u8* _gfx_get_atlas(u32 size) {
	for (u32 i = 0; i < atlas_count; i++) {
		if (atlases[i].size == size) return atlases[i].data;
	}

	if (atlas_count < MAX_ATLASES) {
		u32 muh_bytes  = NUM_CHARS * (size*size);

		u8* start_ptr  = (u8*)atlas_pointer;
		atlas_pointer += (muh_bytes + 3) & ~3;

		atlases[atlas_count++] = (sdfAtlas_t) {size, start_ptr};
		return start_ptr;
	}
	return NULL;
}

void gfxBakeAtlas(u32 fontSize) {
	u8 *atlas_buf = _gfx_get_atlas(fontSize);
	if (!atlas_buf) return;

	// Fine-tuned number
	sfp16_t scale_factor = SFP16FROMINT(1) + (SFP16FROMINT(24) / 100);

	for (int i = 0; i < NUM_CHARS; i++) {
		u8* char_sdf_base = &((u8*)SDF_BUFFER)[i * (SDF_SIZE * SDF_SIZE)];

		for (int py = 0; py < fontSize; py++) {
			sfp16_t norm_y = sfp16Div(SFP16FROMINT(py), SFP16FROMINT(fontSize - 1));
			sfp16_t scaled_norm_y = SFP16HALF + sfp16Mul(norm_y - SFP16HALF, sfp16Div(SFP16FROMINT(1), scale_factor));
			sfp16_t y_pos = sfp16Mul(scaled_norm_y, SFP16FROMINT(SDF_SIZE - 1));

			for (int px = 0; px < fontSize; px++) {
				sfp16_t norm_x = sfp16Div(SFP16FROMINT(px), SFP16FROMINT(fontSize - 1));
				sfp16_t scaled_norm_x = SFP16HALF + sfp16Mul(norm_x - SFP16HALF, sfp16Div(SFP16FROMINT(1), scale_factor));
				sfp16_t x_pos = sfp16Mul(scaled_norm_x, SFP16FROMINT(SDF_SIZE - 1));

				int ix = SFP16GETINT(x_pos);
				int iy = SFP16GETINT(y_pos);
				int fx = SFP16GETDEC(x_pos);
				int fy = SFP16GETDEC(y_pos);

				int cix = ix + 1;
				int ciy = iy + 1;
				if (ciy >= SDF_SIZE) ciy = SDF_SIZE - 1;
				if (cix >= SDF_SIZE) cix = SDF_SIZE - 1;

				int s00 = char_sdf_base[ iy * SDF_SIZE +  ix];
				int s10 = char_sdf_base[ iy * SDF_SIZE + cix];
				int s01 = char_sdf_base[ciy * SDF_SIZE +  ix];
				int s11 = char_sdf_base[ciy * SDF_SIZE + cix];

				int top = s00 + (((s10 - s00) * fx) >> 16);
				int bot = s01 + (((s11 - s01) * fx) >> 16);
				int sam = top + (((bot - top) * fy) >> 16);

				u32 index  = i * (fontSize * fontSize) + (py * fontSize) + px;

				int alpha = CLAMP((sam - 128) * 255, 0, 255);

				atlas_buf[index] = (u8)alpha;
			}
		}
	}
}

void gfxClearGrey(u8 color) {
	memset(gfx_ctxt.fb, color, gfx_ctxt.width * gfx_ctxt.height * 4);
}

void gfxClearPartialGrey(u8 color, u32 pos_y, u32 height) {
	memset(gfx_ctxt.fb + pos_y * gfx_ctxt.stride, color, height * 4 * gfx_ctxt.stride);
}

void gfxClearColor(u32 color) {
	for (u32 i = 0; i < gfx_ctxt.width * gfx_ctxt.height; i++)
		gfx_ctxt.fb[i] = color;
}

void gfxInitCtxt(u32 *fb, u32 width, u32 height, u32 stride) {
	gfx_ctxt.fb     = fb;
	gfx_ctxt.width  = width;
	gfx_ctxt.height = height;
	gfx_ctxt.stride = stride;
}

void gfxConInit() {
	gfx_con.gfxCtxt  = &gfx_ctxt;
	gfx_con.fntsz     = 16;
	gfx_con.x         = 0;
	gfx_con.y         = 0;
	gfx_con.savedx    = 0;
	gfx_con.savedy    = 0;
	gfx_con.fgcol     = COLOR_GREEN;
	gfx_con.fillbg    = 1;
	gfx_con.bgcol     = COLOR_DEFAULT;
	gfx_con.mute      = 0;

	gfx_con_init_done = true;

	gfxRenderSDF();
	gfxBakeAtlas(gfx_con.fntsz);
}

void gfxConSetCol(u32 fgcol, int fillbg, u32 bgcol){
	gfx_con.fgcol  = fgcol;
	gfx_con.fillbg = fillbg;
	gfx_con.bgcol  = bgcol;
}

void gfxConGetPos(u32 *x, u32 *y) {
	*y = gfx_con.y;
	*x = gfx_con.x;
}

void gfxConSetPos(u32 x, u32 y) {
	gfx_con.y = y;
	gfx_con.x = x;

}

void gfxPutsLimit(const char *s, u32 limit){
	if (!s || gfx_con.mute)
		return;

	u32 len = strlen(s);
	u32 char_limit = limit / gfx_con.fntsz;

	if (char_limit > len) {
		gfxPuts(s);
		return;
	}

	if (len > char_limit)
		char_limit -= 4;

	for (int i = 0; i < MIN(len, char_limit); i++)
		gfxPutc(s[i]);

	if (len > char_limit + 4)
		gfxPuts("... ");
}

void gfxConSetFontSize(u32 fontSize) {
	gfx_con.fntsz = fontSize;
	gfxBakeAtlas(fontSize);
}

void __attribute__((target("arm"))) __attribute__((optimize("Os"))) gfxPutc(char c) {
	if unlikely(c <= 31 || c >= 129) {
		if (c == '\n') {
			gfx_con.x = 0;
			gfx_con.y += gfx_con.fntsz;
			if (gfx_con.y > gfx_ctxt.height - gfx_con.fntsz)
				gfx_con.y = 0;
		}
		return;
	}
	u32 sz    = gfx_con.fntsz;

	u8 * restrict data = &_gfx_get_atlas(sz)[(c - 32) * (sz * sz)];
	u32* restrict fb   = gfx_ctxt.fb + gfx_con.x + (gfx_con.y * gfx_ctxt.stride);

	u32 mask  = 0x00FF00FF;

	u32 fg    = gfx_con.fgcol;
	u32 fg_rb =  fg       & mask;
	u32 fg_ag = (fg >> 8) & mask;

	u32 bg    = gfx_con.bgcol;
	u32 bg_rb =  bg       & mask;
	u32 bg_ag = (bg >> 8) & mask;

	for (u32 y = 0; y < sz; y++) {
		for (u32 x = 0; x < sz; x++) {
			u8 alpha = *data++;

			if likely(alpha == 0)   { *fb++ = bg; continue;};
			if likely(alpha == 255) { *fb++ = fg; continue;};

			// TODO: change to one mult and test.
			// out = bg + ((fg-bg) * alpha) / 256
			u32 rb = (bg_rb * (255 - alpha) + fg_rb * alpha) >> 8;
			u32 ag = (bg_ag * (255 - alpha) + fg_ag * alpha);

			*fb++ = (rb & mask) | (ag & ~mask);
		}
		fb += gfx_ctxt.stride - sz;
	}
	gfx_con.x += sz;
}

void gfxPuts(const char *s) {
	if (!s || !gfx_con_init_done || gfx_con.mute)
		return;

	for (; *s; s++)
		gfxPutc(*s);
}

void __attribute__((target("arm"))) __attribute__((optimize("Os"))) gfxPutcSmall(char c) {
	u32 sz = 8;
	if unlikely(c <= 33 || c >= 129) {
		if (c == '\n') {
			gfx_con.x = 0;
			gfx_con.y += sz;
			if (gfx_con.y > gfx_ctxt.height - sz)
				gfx_con.y = 0;
		}
		return;
	}

	u8 * restrict data = &_gfx_get_atlas(sz)[(c - 32) * (sz * sz)];
	u32* restrict fb   = gfx_ctxt.fb + gfx_con.x + (gfx_con.y * gfx_ctxt.stride);

	u32 mask  = 0x00FF00FF;

	u32 fg    = gfx_con.fgcol;
	u32 fg_rb =  fg       & mask;
	u32 fg_ag = (fg >> 8) & mask;

	u32 bg    = gfx_con.bgcol;
	u32 bg_rb =  bg       & mask;
	u32 bg_ag = (bg >> 8) & mask;

	for (u32 y = 0; y < sz; y++) {
		for (u32 x = 0; x < sz; x++) {
			u8 alpha = *data++;

			if likely(alpha == 0)   { *fb++ = bg; continue;};
			if likely(alpha == 255) { *fb++ = fg; continue;};

			// TODO: change to one mult and test.
			// out = bg + ((fg-bg) * alpha) / 256
			u32 rb = (bg_rb * (255 - alpha) + fg_rb * alpha) >> 8;
			u32 ag = (bg_ag * (255 - alpha) + fg_ag * alpha);

			*fb++ = (rb & mask) | (ag & ~mask);
		}
		fb += gfx_ctxt.stride - sz;
	}
	gfx_con.x += sz;
}

void gfxPutsSmall(const char *s) {
	if (!s || !gfx_con_init_done || gfx_con.mute)
		return;

	for (; *s; s++)
		gfxPutcSmall(*s);
}

static void _gfx_putn(u32 v, int base, char fill, int fcnt) {
	static const char digits[] = "0123456789ABCDEF";

	char *p;
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

	 p = buf + 64;
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
	if (!gfx_con_init_done || gfx_con.mute)
		return;

	u32 newLineConfig = 0;
	int fill, fcnt;

	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			fill = 0;
			fcnt = 0;
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
					gfx_con.x = newLineConfig;
					break;
				case 'N':
					newLineConfig = va_arg(ap, u32);
					gfx_con.x = newLineConfig;
					break;
				case 'p':
				case 'P':
				case 'x':
				case 'X':
					_gfx_putn(va_arg(ap, u32), 16, fill, fcnt);
					break;
				case 'k':
					gfx_con.fgcol = va_arg(ap, u32);
					break;
				case 'K':
					gfx_con.bgcol = va_arg(ap, u32);
					gfx_con.fillbg = 1;
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

void gfx_printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gfxVPrintF(fmt, ap);
	va_end(ap);
}


void gfxCPuts(u32 color, const char *s) {
	gfx_con.fgcol = color;
	gfxPuts(s);
	gfxPutc('\n');
	gfx_con.fgcol = COLOR_DEFAULT;
}

// TODO: Look into difference between this (hekates) and TEs

void gfxHexDump(u32 base, const void *buf, u32 len) {
	if (!gfx_con_init_done || gfx_con.mute)
		return;

	u8 *buff = (u8 *)buf;

	u8 prevFontSize = gfx_con.fntsz;
	// gfx_con.fntsz = 8;
	for (u32 i = 0; i < len; i++) {
		if (i % 0x10 == 0) {
			if (i != 0) {
				gfxPuts("| ");
				for (u32 j = 0; j < 0x10; j++) {
					u8 c = buff[i - 0x10 + j];
					if (c >= 32 && c <= 126)
						gfxPutc(c);
					else
						gfxPutc('.');
				}
				gfxPutc('\n');
			}
			gfx_printf("%08x: ", base + i);
		}
		gfx_printf("%02x", buff[i]);
		if (i == len - 1) {
			int ln = len % 0x10 != 0;
			u32 k = 0x10 - 1;
			if (ln) {
				k = (len & 0xF) - 1;
				for (u32 j = 0; j < 0x10 - k; j++)
					gfxPuts("   ");
			}
			gfxPuts("| ");
			for (u32 j = 0; j < (ln ? k : k + 1); j++) {
				u8 c = buff[i - k + j];
				if (c >= 32 && c <= 126)
					gfxPutc(c);
				else
					gfxPutc('.');
			}
			gfxPutc('\n');
		}
	}
	gfxPutc('\n');
	gfx_con.fntsz = prevFontSize;
}

void gfxSetPixel(u32 x, u32 y, u32 color) {
	gfx_ctxt.fb[x + y * gfx_ctxt.stride] = color;
}

static int _abs(int x) {
	if (x < 0) return -x;
	return x;
}

void gfxLine(int x0, int y0, int x1, int y1, u32 color) {
	int dx  = _abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy  = _abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	while (1) {
		gfxSetPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 >-dx) {
			err -= dy;
			x0  += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0  += sy;
		}
	}
}

void gfxSetRectPitch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2) {
	u32 *ptr = (u32 *)buf;
	u32 line_size = pos_x2 - pos_x + 1;
	//ptr = gfx_debug_rect(buf, pos_x, pos_y, pos_x2, pos_y2);
	for (u32 y = pos_y; y <= pos_y2; y++) {
		memcpy(&fb[pos_x + y * stride], ptr, line_size * sizeof(u32));
		ptr += line_size;
	}
}

void gfxSetRectLandPtch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2) {
	u32 *ptr = (u32 *)buf;

	u32 pixels_w = pos_x2 - pos_x + 1;

	if (!(pixels_w % 8)) {
		for (u32 y = pos_y; y <= pos_y2; y++)
			for (u32 x = pos_x; x <= pos_x2; x += 8) {
				u32 *fbx = &fb[x * stride + y];

				fbx[0]          = *ptr++;
				fbx[stride]     = *ptr++;
				fbx[stride * 2] = *ptr++;
				fbx[stride * 3] = *ptr++;
				fbx[stride * 4] = *ptr++;
				fbx[stride * 5] = *ptr++;
				fbx[stride * 6] = *ptr++;
				fbx[stride * 7] = *ptr++;
			}
	} else {
		for (u32 y = pos_y; y < (pos_y2 + 1); y++)
			for (u32 x = pos_x; x < (pos_x2 + 1); x++)
				fb[x * stride + y] = *ptr++;
	}
}

void gfxSetRectLandBlock(u32 *fb, const u32 *buf, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2) {
	u32 *ptr = (u32 *)buf;
	u32 GOB_address = 0;
	u32 addr = 0;
	u32 x2 = 0;

	// Optimized
	u32 image_width_in_gobs = 655360; //1280
	for (u32 y = pos_y; y <= pos_y2; y++) {
		for (u32 x = pos_x; x <= pos_x2; x++) {
			GOB_address = (y >> 7) * image_width_in_gobs + ((x >> 4) << 13) + (((y % 128) >> 3) << 9);

			x2 = x << 2;
			addr = GOB_address
				+ (((x2 % 64) >> 5) << 8)
				+ (((y % 8) >> 1) << 6)
				+ (((x2 % 32) >> 4) << 5)
				+ ((y % 2) << 4) + (x2 % 16);

			*(u32 *)(fb + (addr >> 2)) = *ptr++;
		}
	}

	// Proper
	// u32 block_height = 16;
	// u32 image_width_in_gobs = (512 * block_height * 1280 * 4) / 64;
	// for (u32 y = pos_y; y <= pos_y2; y++)
	// {
	// 	for (int x = pos_x; x <= pos_x2; x++)
	// 	{
	// 		GOB_address = (y / (8 * block_height)) * image_width_in_gobs + ((x * 4 / 64) * 512 * block_height) + ((y % (8 * block_height) / 8) * 512);

	// 		x2 = x << 2;
	// 		addr = GOB_address
	// 			+ (((x2 % 64) >> 5) << 8)
	// 			+ (((y % 8) >> 1) << 6)
	// 			+ (((x2 % 32) >> 4) << 5)
	// 			+ ((y % 2) << 4) + (x2 % 16);

	// 		*(u32 *)(gfx_ctxt.fb + (addr >> 2)) = *ptr++;
	// 	}
	// }
}
void gfxBox(int x0, int y0, int x1, int y1, u32 color){
	for (int x = x0; x < x1 + 1; x++){
		for (int y = y0; y < y1 + 1; y++){
			gfxSetPixel(x, y, color);
		}
	}
}

void gfxBoxGrey(int x0, int y0, int x1, int y1, u8 shade){
	for (int y = y0; y >= y1; y--){
		memset(gfx_ctxt.fb + y * gfx_ctxt.stride + x0, shade, (x1 - x0 + 1) * 4);
	}
}

void gfxSetRectRGB(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y) {
	u32 pos = 0;
	for (u32 y = pos_y; y < (pos_y + size_y); y++) {
		for (u32 x = pos_x; x < (pos_x + size_x); x++) {
			gfx_ctxt.fb[y + x * gfx_ctxt.stride] = buf[pos + 2] | buf[pos + 1] << 8 | buf[pos] << 16;
			pos += 3;
		}
	}
}

void gfxSetRectARGB(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y) {
	u32 *ptr = (u32 *)buf;
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = *ptr++;
}

void gfxRenderBMPARGB(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y) {
	for (u32 y = pos_y; y < (pos_y + size_y); y++) {
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = buf[(size_y + pos_y - 1 - y ) * size_x + x - pos_x];
	}
}