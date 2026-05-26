//
// Created by yes on 5/22/26.
//

#ifndef _FIXEDPOINT8X8_H
#define _FIXEDPOINT8X8_H

#include <bdk.h>

// I know, I know. Putting an 8.8 fixed point implementation in a 32 bit wide type. Its a crime.
// I had to. My hands were tied. I had no other option. I was forced. What did you want me to do?
// You wouldn't send someone to prison for steeling bread to feed themselves... right?
// - auggiethecat right eb
typedef s32 sfp8_t;

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

#endif //_FIXEDPOINT8X8_H
