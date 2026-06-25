//
// Created by auggiethecat on 5/3/26.
//

#ifndef _HID_H
#define _HID_H

#include "bdk.h"

#define JoyY      BIT(0)
#define JoyX      BIT(1)
#define JoyB      BIT(2)
#define JoyA      BIT(3)
#define JoySR_R   BIT(4)
#define JoySL_R   BIT(5)
#define JoyR      BIT(6)
#define JoyZR     BIT(7)

#define JoyMINUS  BIT(8)
#define JoyPLUS   BIT(9)
#define JoyR3     BIT(10)
#define JoyL3     BIT(11)
#define JoyHOME   BIT(12)
#define JoyCAP    BIT(13)
#define JoyPAD    BIT(14)
#define JoyWIRED  BIT(15)

#define JoyLDown  BIT(16)
#define JoyLUp    BIT(17)
#define JoyLRight BIT(18)
#define JoyLLeft  BIT(19)
#define JoySR_L   BIT(20)
#define JoySL_L   BIT(21)
#define JoyL      BIT(22)
#define JoyZL     BIT(23)

#define BtnPow    BIT(24)
#define BtnVolP   BIT(25)
#define BtnVolM   BIT(26)

#define JoyRDown  BIT(27)
#define JoyRUp    BIT(28)
#define JoyRRight BIT(29)
#define JoyRLeft  BIT(30)
#define JOYUNUSED BIT(31)

#define WAITBUTTONS (JoyY | JoyX | JoyB | JoyA | JoyLDown | JoyLUp | JoyLRight | JoyLLeft)

typedef struct _input_t {
    union {
        struct {
            // Joy-Con (R).
            u32 y     :1;
            u32 x     :1;
            u32 b     :1;
            u32 a     :1;
            u32 sr_r  :1;
            u32 sl_r  :1;
            u32 r     :1;
            u32 zr    :1;

            // Shared
            u32 minus :1;
            u32 plus  :1;
            u32 r3    :1;
            u32 l3    :1;
            u32 home  :1;
            u32 cap   :1;
            u32 pad   :1;
            u32 wired :1;

            // Joy-Con (L).
            u32 down  :1;
            u32 up    :1;
            u32 right :1;
            u32 left  :1;
            u32 sr_l  :1;
            u32 sl_l  :1;
            u32 l     :1;
            u32 zl    :1;

            u32 power :1;
            u32 volp  :1;
            u32 volm  :1;

            u32 rDown :1;
            u32 rUp   :1;
            u32 rRight:1;
            u32 rLeft :1;

            u32 unused:1;
        };
        u32 buttons;
    };
} input_t;

void     hidInit();
input_t *hidRead();
input_t *hidWait();
input_t *hidWaitMask(u32 mask);
bool     hidConnected();

#endif //_HID_H