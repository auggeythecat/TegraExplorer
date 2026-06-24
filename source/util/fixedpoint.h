//
// Created by yes on 5/22/26.
//

#ifndef _FIXEDPOINT8X8_H
#define _FIXEDPOINT8X8_H

#include <bdk.h>

typedef s16 sfp8_t;

#define SFP8BITS       (8)
#define SFP8ONE        (1 << SFP8BITS)
#define SFP8HALF       (1 << (SFP8BITS - 1))

#define SFP8GETINT(k)  ((k) >> SFP8BITS)
#define SFP8FROMINT(k) ((sfp8_t)(k) << SFP8BITS)

#define SFP8GETDEC(k)  ((sfp8_t)(k) & (0x00FF))

sfp8_t sfp8_lrp(sfp8_t a, sfp8_t b, sfp8_t t) { return a + (sfp8_t)(((s32)t * (b - a) + SFP8HALF) >> SFP8BITS); }
sfp8_t sfp8_mul(sfp8_t a, sfp8_t b) { return (sfp8_t)(((s32)a * (s32)b + SFP8HALF) >> SFP8BITS); }
sfp8_t sfp8_div(sfp8_t a, sfp8_t b) { return (sfp8_t)(((s32)a << SFP8BITS) / b); }
sfp8_t sfp8_add(sfp8_t a, sfp8_t b) { return a + b; }
sfp8_t sfp8_sub(sfp8_t a, sfp8_t b) { return a - b; }

typedef s32 sfp16_t;

#define SFP16BITS       (16)
#define SFP16ONE        (1 << SFP16BITS)
#define SFP16HALF       (1 << (SFP16BITS - 1))

#define SFP16GETINT(k)  ((k) >> SFP16BITS)
#define SFP16FROMINT(k) ((sfp16_t)(k) << SFP16BITS)

#define SFP16GETDEC(k)  ((sfp16_t)(k) & (0xFFFF))

sfp16_t sfp16_lrp(sfp16_t a, sfp16_t b, sfp16_t t) { return a + (sfp16_t)(((s64)t * (b - a) + SFP16HALF) >> SFP16BITS); }
sfp16_t sfp16_mul(sfp16_t a, sfp16_t b) { return (sfp16_t)(((s64)a * (s64)b + SFP16HALF) >> SFP16BITS); }
sfp16_t sfp16_div(sfp16_t a, sfp16_t b) { return (sfp16_t)(((s64)a << SFP16BITS) / b); }
sfp16_t sfp16_add(sfp16_t a, sfp16_t b) { return a + b; }
sfp16_t sfp16_sub(sfp16_t a, sfp16_t b) { return a - b; }

#endif //_FIXEDPOINT8X8_H
