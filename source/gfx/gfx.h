#ifndef _GFX_H_
#define _GFX_H_

#include <bdk.h>
#define GFX_COL_KEEP 0xFFFE
#define GFX_COL_AUTO 0xFFFF

#define COLOR_RED          0xFFE70000
#define COLOR_ORANGE       0xFFFF8C00
#define COLOR_YELLOW       0xFFFFFF40
#define COLOR_GREEN        0xFF40FF00
#define COLOR_BLUE         0xFF00DDFF
#define COLOR_VIOLET       0xFF8040FF

#define COLOR_WHITE        0xFFFFFFFF
#define COLOR_LIGHTER_GREY 0xFFD0D0D0
#define COLOR_LIGHT_GREY   0xFFA0A0A0
#define COLOR_DARK_GREY    0xFF606060
#define COLOR_DARKER_GREY  0xFF303030
#define COLOR_BLACK        0xFF000000

#define EPRINTF(text) gfx_printf("%k"text"%k\n", COLOR_RED, COLOR_LIGHTER_GREY)
#define EPRINTFARGS(text, args...) gfx_printf("%k"text"%k\n", COLOR_RED, args, COLOR_LIGHTER_GREY)

#define WPRINTF(text) gfx_printf("%k"text"%k\n", COLOR_YELLOW, COLOR_LIGHTER_GREY)
#define WPRINTFARGS(text, args...) gfx_printf("%k"text"%k\n", COLOR_YELLOW, args, COLOR_LIGHTER_GREY)

typedef struct _gfx_ctxt_t
{
    u32 *fb;
    u32 width;
    u32 height;
    u32 stride;
} gfx_ctxt_t;

typedef struct _gfx_con_t
{
    gfx_ctxt_t *gfx_ctxt;
    u32 fntsz;
    u32 x;
    u32 y;
    u32 savedx;
    u32 savedy;
    u32 fgcol;
    int fillbg;
    u32 bgcol;
    bool mute;
} gfx_con_t;

extern gfx_ctxt_t gfx_ctxt;
extern gfx_con_t gfx_con;

#define YLEFT 1279

// TODO: How important is c/col/column?

void gfx_init_ctxt(u32 *fb, u32 width, u32 height, u32 stride);
void gfx_clear_grey(u8 color);
void gfx_clear_color(u32 color);
void gfx_con_init();
void gfx_con_setcol(u32 fgcol, int fillbg, u32 bgcol);
void gfx_con_getpos(u32 *x, u32 *y);
void gfx_con_setpos(u32 x, u32 y);

void gfx_putc(char c);
void gfx_puts(const char *s);
void gfx_cputs(u32 color, const char *s);
void gfx_vprintf(const char *fmt, va_list ap);
void gfx_printf(const char *fmt, ...);
void gfx_hexdump(u32 base, const void *buf, u32 len);

void gfx_set_pixel(u32 x, u32 y, u32 color);
void gfx_set_pixel_horz(int x, int y, u32 color);
void gfx_line(int x0, int y0, int x1, int y1, u32 color);

void gfx_set_rect_argb(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);
void gfx_render_bmp_argb(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);
void gfx_box(int x0, int y0, int x1, int y1, u32 color);
void gfx_boxGrey(int x0, int y0, int x1, int y1, u8 shade);

// TODO: What do these do?
void gfx_set_rect_pitch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2);
void gfx_set_rect_land_pitch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2);
void gfx_set_rect_land_block(u32 *fb, const u32 *buf, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2);



// TODO: Are these needed?
void gfx_clear_partial_grey(u8 color, u32 pos_x, u32 height);
void gfx_vprintf(const char *fmt, va_list ap);
void gfx_wputs(const char *s);
void gfx_eputs(const char *s);
void gfx_hexdiff(u32 base, const u8 *buf1, const u8 *buf2, u32 len);
void gfx_puts_limit(const char *s, u32 limit);
void gfx_puts_small(const char *s);
void gfx_putc_small(char c);
void gfx_put_small_sep();
void gfx_put_big_sep();
// What are these for?
void gfx_set_rect_grey(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);
void gfx_set_rect_rgb(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);

#endif
