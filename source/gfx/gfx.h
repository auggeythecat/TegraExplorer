#ifndef _GFX_H_
#define _GFX_H_

#include <bdk.h>

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define GFX_COL_KEEP 0xFFFE
#define GFX_COL_AUTO 0xFFFF

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

#define EPRINTF(text) gfx_printf("%k"text"%k\n", COLOR_RED, COLOR_DEFAULT)
#define EPRINTFARGS(text, args...) gfx_printf("%k"text"%k\n", COLOR_RED, args, COLOR_DEFAULT)

#define WPRINTF(text) gfx_printf("%k"text"%k\n", COLOR_YELLOW, COLOR_DEFAULT)
#define WPRINTFARGS(text, args...) gfx_printf("%k"text"%k\n", COLOR_YELLOW, args, COLOR_DEFAULT)

#define RGBTOCOLOR(r, g, b) (b << 16 | g << 8 | r << 0)
#define GREYTOCOLOR(s)      (s << 16 | s << 8 | s << 0)
#define COLORTORGB(color)   (color & 0x00FFFFFF)
#define COLORTOGREY(color)  (color & 0x000000FF) // TODO: proper conversion. For now, just only input grey colors.
#define INVERTCOLOR(color)  (~(color & 0x00FFFFFF))
#define SETCOLOR(fg, bg) gfxConSetCol(fg, 1, bg)
#define RESETCOLOR SETCOLOR(COLOR_WHITE, COLOR_DEFAULT);


#define NATIVE_FONT_SIZE 8
#define PADDING          1
#define SDF_SIZE         (NATIVE_FONT_SIZE + (PADDING * 2))
#define INNER_SIZE       (SDF_SIZE - (PADDING * 2))

#define MAX_ATLASES      16
#define NUM_CHARS        98

#define SDF_BUFFER       NYX_RES_ADDR

#define GFX_CHAR_SPACE    0
#define GFX_CHAR_FOLDER   127
#define GFX_CHAR_FILE     128
#define GFX_CHAR_CHARGING 129

//TODO: move to a util file or something, this doesn't belong here.
#define    CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))
#define CLAMPMIN(val, min)      ((val) < (min) ? (min) :  (val))
#define CLAMPMAX(val, max)                               ((val) > (max) ? (max) : (val))

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

// Prevent bd. TODO: Better solution?

extern gfxCtxt_t gfx_ctxt;
extern gfxCon_t  gfx_con;

void gfxInitCtxt(u32 *fb, u32 width, u32 height, u32 stride);
void gfxClearGrey(u8 color);
void gfxClearPartialGrey(u8 color, u32 pos_x, u32 height);
void gfxClearColor(u32 color);
void gfxConInit();
void gfxConSetCol(u32 fgcol, int fillbg, u32 bgcol);
void gfxConGetPos(u32 *x, u32 *y);
void gfxConSetPos(u32 x, u32 y);

void gfxPutc(char c);
void gfxPuts(const char *s);
void gfxCPuts(u32 color, const char *s);
void gfxVPrintF(const char *fmt, va_list ap);
void gfx_printf(const char *fmt, ...); // must keep compatability with fatfs. TODO: Better solution?
void gfxPutsLimit(const char *s, u32 limit);
void gfxHexDump(u32 base, const void *buf, u32 len);

void gfxSetPixel(u32 x, u32 y, u32 color);
void gfxSetPixelHorz(int x, int y, u32 color);
void gfxLine(int x0, int y0, int x1, int y1, u32 color);

void gfxSetRectARGB(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);
void gfxRenderBMPARGB(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);
void gfxBox(int x0, int y0, int x1, int y1, u32 color);
void gfxBoxGrey(int x0, int y0, int x1, int y1, u8 shade);

// TODO: What do these do?
void gfxSetRectPitch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2);
void gfxSetRectLandPtch(u32 *fb, const u32 *buf, u32 stride, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2);
void gfxSetRectLandBlock(u32 *fb, const u32 *buf, u32 pos_x, u32 pos_y, u32 pos_x2, u32 pos_y2);


// TODO: Are these needed?
void gfxHexDiff(u32 base, const u8 *buf1, const u8 *buf2, u32 len);
void gfxPutsSmall(const char *s);
void gfxPutcSmall(char c);
// What are these for?
void gfxSetRectGrey(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);
void gfxSetRectRGB(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y);

void gfxRenderSDF();
void gfxBakeAtlas(u32 fontSize);
void gfxConSetFontSize(u32 fontSize);

#endif
