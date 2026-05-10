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

gfx_ctxt_t gfx_ctxt;
gfx_con_t gfx_con;

static bool gfx_con_init_done = false;

static const u8 _gfx_font[] = {
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
    0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,   // U+007C (|)
    0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00,   // U+007D (})
    0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+007E (~)
	0x00, 0x0C, 0x12, 0x7E, 0x42, 0x42, 0x7E, 0x00,   //   (folder)
	0x00, 0x0E, 0x12, 0x22, 0x22, 0x22, 0x3E, 0x00,   //     (file)
	0x00, 0x08, 0x0C, 0x0E, 0x7E, 0x70, 0x30, 0x10,   // (Charging)
};

u32 YLeftConfig = YLEFT;

void gfx_clear_grey(u8 color)
{
	memset(gfx_ctxt.fb, color, gfx_ctxt.width * gfx_ctxt.height * 4);
}

void gfx_clear_color(u32 color)
{
	for (u32 i = 0; i < gfx_ctxt.width * gfx_ctxt.height; i++)
		gfx_ctxt.fb[i] = color;
}

void gfx_init_ctxt(u32 *fb, u32 width, u32 height, u32 stride)
{
	gfx_ctxt.fb = fb;
	gfx_ctxt.width = width;
	gfx_ctxt.height = height;
	gfx_ctxt.stride = stride;

	gfx_clear_grey(0); // TODO: should this be here?
}

void gfx_con_init()
{
	gfx_con.gfx_ctxt = &gfx_ctxt;
	gfx_con.fntsz = 16;
	gfx_con.x = 0;
	gfx_con.y = 0;
	gfx_con.savedx = 0;
	gfx_con.savedy = 0;
	gfx_con.fgcol = COLOR_WHITE;
	gfx_con.fillbg = 1;
	gfx_con.bgcol = COLOR_LIGHT_GREY;
	gfx_con.mute = 0;

	gfx_con_init_done = true;
}

void gfx_con_setcol(u32 fgcol, int fillbg, u32 bgcol)
{
	gfx_con.fgcol = fgcol;
	gfx_con.fillbg = fillbg;
	gfx_con.bgcol = bgcol;
}

void gfx_con_getpos(u32 *x, u32 *y)
{
	*x = YLEFT - gfx_con.y;
	*y = gfx_con.x;
}

void gfx_con_setpos(u32 x, u32 y)
{
	gfx_con.x = y;
	gfx_con.y = YLEFT - x;

}

// TODO: See if I can make this make more sense

void gfx_putc(char c)
{
	// Duplicate code for performance reasons.
	switch (gfx_con.fntsz)
	{
	case 16:
		if (c >= 32 && c <= 129)
		{
			u8 *cbuf = (u8 *)&_gfx_font[8 * (c - 32)];
			u32 *fb = gfx_ctxt.fb + gfx_con.x + gfx_con.y * gfx_ctxt.stride;

			for (u32 i = 0; i < 16; i+=2)
			{
				u8 v = *cbuf;
				for (u32 t = 0; t < 8; t++){
					if (v & 1 || gfx_con.fillbg){
						u32 setColor = (v & 1) ? gfx_con.fgcol : gfx_con.bgcol;
						*fb = setColor;
						*(fb + 1) = setColor;
						*(fb - gfx_ctxt.stride) = setColor;
						*(fb - gfx_ctxt.stride + 1) = setColor;
					}
					v >>= 1;
					fb -= gfx_ctxt.stride * 2;
				}
				fb += gfx_ctxt.stride * 16 + 2;
				cbuf++;
				/*
				for (u32 k = 0; k < 2; k++)
				{
					for (u32 j = 0; j < 8; j++)
					{
						if (v & 1)
						{
							*fb = gfx_con.fgcol;
							fb -= gfx_ctxt.stride;
							*fb = gfx_con.fgcol;
						}
						else if (gfx_con.fillbg)
						{
							*fb = gfx_con.bgcol;
							fb -= gfx_ctxt.stride;
							*fb = gfx_con.bgcol;
						}
						else
							fb -= gfx_ctxt.stride;
						v >>= 1;
						fb -= gfx_ctxt.stride;
					}
					//fb += gfx_ctxt.stride - 16;
					//fb = fbtop + 2;
					fb += (gfx_ctxt.stride * 16) + 1;
					v = *cbuf;
				}
				cbuf++;
				*/
			}

			gfx_con.y -= 16;
			if (gfx_con.y < 16){
				gfx_con.y = YLeftConfig;
				gfx_con.x += 16;
				if (gfx_con.x > 719)
					gfx_con.x = 0;
			}
		}
		else if (c == '\n')
		{
			gfx_con.y = YLeftConfig;
			gfx_con.x += 16;
			if (gfx_con.x > gfx_ctxt.width - 16)
				gfx_con.x = 0;
		}
		else if (c == '\e')
			gfx_con.y = 575;
		else if (c == '\a')
			gfx_con.y = 639;
		else if (c == '\r')
			gfx_con.y = YLeftConfig;

		break;
	case 8:
	default:
		if (c >= 30 && c <= 129)
		{
			u8 *cbuf = (u8 *)&_gfx_font[8 * (c - 32)];
			u32 *fb = gfx_ctxt.fb + gfx_con.x + gfx_con.y * gfx_ctxt.stride;
			for (u32 i = 0; i < 8; i++)
			{
				u8 v = *cbuf++;
				for (u32 j = 0; j < 8; j++)
				{
					if (v & 1)
						*fb = gfx_con.fgcol;
					else if (gfx_con.fillbg)
						*fb = gfx_con.bgcol;
					v >>= 1;
					fb -= gfx_ctxt.stride;
				}
				fb += (gfx_ctxt.stride * 8) + 1;
			}

			gfx_con.y -= 8;
			if (gfx_con.y < 8){
				gfx_con.y = YLeftConfig;
				gfx_con.x += 8;
			}

		}
		else if (c == '\n')
		{
			gfx_con.y = YLeftConfig;
			gfx_con.x += 8;
			if (gfx_con.x > gfx_ctxt.width - 8)
				gfx_con.x = 0;
		}
		else if (c == '\e')
			gfx_con.y = 575;
		else if (c == '\a')
			gfx_con.y = 639;
		else if (c == '\r')
			gfx_con.y = YLeftConfig;

		break;
	}
}

void gfx_puts(const char *s)
{
	if (!s || !gfx_con_init_done || gfx_con.mute)
		return;

	for (; *s; s++)
		gfx_putc(*s);
}

static void _gfx_putn(u32 v, int base, char fill, int fcnt)
{
	static const char digits[] = "0123456789ABCDEF";

	char *p;
	char buf[65];
	int c = fcnt;
	bool negative = false;

	if (base != 10 && base != 16)
		return;

	// Account for negative numbers.
	if (base == 10 && v & 0x80000000)
	{
		negative = true;
		v = (int)v * -1;
		c--;
	}

	p = buf + 64;
	*p = 0;
	do
	{
		c--;
		*--p = digits[v % base];
		v /= base;
	} while (v);

	if (negative)
		*--p = '-';

	if (fill != 0)
	{
		while (c > 0 && p > buf)
		{
			*--p = fill;
			c--;
		}
	}

	gfx_puts(p);
}

void gfx_printf(const char *fmt, ...)
{
	if (gfx_con.mute)
		return;

	va_list ap;
	int fill, fcnt;

	va_start(ap, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			fill = 0;
			fcnt = 0;
			if ((*fmt >= '0' && *fmt <= '9') || *fmt == ' ')
			{
				fcnt = *fmt;
				fmt++;
				if (*fmt >= '0' && *fmt <= '9')
				{
					fill = fcnt;
					fcnt = *fmt - '0';
					fmt++;
				}
				else
				{
					fill = ' ';
					fcnt -= '0';
				}
			}
			switch(*fmt)
			{
				case 'c':
					gfx_putc(va_arg(ap, u32));
					break;
				case 's':
					gfx_puts(va_arg(ap, char *));
					break;
				case 'd':
					_gfx_putn(va_arg(ap, u32), 10, fill, fcnt);
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
					gfx_putc('%');
					break;
				case '\0':
					goto out;
				default:
					gfx_putc('%');
					gfx_putc(*fmt);
					break;
			}
		}
		else
			gfx_putc(*fmt);
		fmt++;
	}

	out:
	va_end(ap);
}


void gfx_cputs(u32 color, const char *s)
{
	gfx_con.fgcol = color;
	gfx_puts(s);
	gfx_putc('\n');
	gfx_con.fgcol = COLOR_LIGHTER_GREY;
}

// TODO: Look into difference between this (hekates) and TEs

void gfx_hexdump(u32 base, const void *buf, u32 len)
{
	if (!gfx_con_init_done || gfx_con.mute)
		return;

	u8 *buff = (u8 *)buf;

	u8 prevFontSize = gfx_con.fntsz;
	gfx_con.fntsz = 8;
	for (u32 i = 0; i < len; i++)
	{
		if (i % 0x10 == 0)
		{
			if (i != 0)
			{
				gfx_puts("| ");
				for (u32 j = 0; j < 0x10; j++)
				{
					u8 c = buff[i - 0x10 + j];
					if (c >= 32 && c <= 126)
						gfx_putc(c);
					else
						gfx_putc('.');
				}
				gfx_putc('\n');
			}
			gfx_printf("%08x: ", base + i);
		}
		gfx_printf("%02x ", buff[i]);
		if (i == len - 1)
		{
			int ln = len % 0x10 != 0;
			u32 k = 0x10 - 1;
			if (ln)
			{
				k = (len & 0xF) - 1;
				for (u32 j = 0; j < 0x10 - k; j++)
					gfx_puts("   ");
			}
			gfx_puts("| ");
			for (u32 j = 0; j < (ln ? k : k + 1); j++)
			{
				u8 c = buff[i - k + j];
				if (c >= 32 && c <= 126)
					gfx_putc(c);
				else
					gfx_putc('.');
			}
			gfx_putc('\n');
		}
	}
	gfx_putc('\n');
	gfx_con.fntsz = prevFontSize;
}

void gfx_set_pixel(u32 x, u32 y, u32 color)
{
	gfx_ctxt.fb[x + y * gfx_ctxt.stride] = color;
}

static int _abs(int x)
{
	if (x < 0)
		return -x;
	return x;
}

void gfx_set_pixel_horz(int x, int y, u32 color) {
	*(gfx_ctxt.fb + (YLEFT - x) * gfx_ctxt.stride + y) = color;
}

void gfx_line(int x0, int y0, int x1, int y1, u32 color)
{
	int dx = _abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = _abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	while (1)
	{
		gfx_set_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 >-dx)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void gfx_set_rect_pitch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2)
{
	u32 *ptr = (u32 *)buf;
	u32 line_size = pos_x2 - pos_x + 1;
	//ptr = gfx_debug_rect(buf, pos_x, pos_y, pos_x2, pos_y2);
	for (u32 y = pos_y; y <= pos_y2; y++)
	{
		memcpy(&fb[pos_x + y * stride], ptr, line_size * sizeof(u32));
		ptr += line_size;
	}
}

void gfx_set_rect_land_pitch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2)
{
	u32 *ptr = (u32 *)buf;

	u32 pixels_w = pos_x2 - pos_x + 1;

	if (!(pixels_w % 8))
	{
		for (u32 y = pos_y; y <= pos_y2; y++)
			for (u32 x = pos_x; x <= pos_x2; x += 8)
			{
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
	}
	else
	{
		for (u32 y = pos_y; y < (pos_y2 + 1); y++)
			for (u32 x = pos_x; x < (pos_x2 + 1); x++)
				fb[x * stride + y] = *ptr++;
	}
}

void gfx_set_rect_land_block(u32 *fb, const u32 *buf, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2)
{
	u32 *ptr = (u32 *)buf;
	u32 GOB_address = 0;
	u32 addr = 0;
	u32 x2 = 0;

	// Optimized
	u32 image_width_in_gobs = 655360; //1280
	for (u32 y = pos_y; y <= pos_y2; y++)
	{
		for (u32 x = pos_x; x <= pos_x2; x++)
		{
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

void gfx_box(int x0, int y0, int x1, int y1, u32 color){
	for (int y = (YLEFT - x0); y >= (YLEFT - x1); y--){
		for (int x = y0; x <= y1; x++){
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = color;
		}
	}
}

void gfx_set_rect_rgb(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
	u32 pos = 0;
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
	{
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
		{
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = buf[pos + 2] | (buf[pos + 1] << 8) | (buf[pos] << 16);
			pos+=3;
		}
	}
}

void gfx_set_rect_argb(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
	u32 *ptr = (u32 *)buf;
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = *ptr++;
}

void gfx_render_bmp_argb(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
	{
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = buf[(size_y + pos_y - 1 - y ) * size_x + x - pos_x];
	}
}