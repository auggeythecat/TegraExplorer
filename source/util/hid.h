//
// Created by auggiethecat on 5/3/26.
//

#ifndef _HID_H
#define _HID_H

#include "bdk.h"

#define JOYSTICK_THRESHOLD 500

#define RE_DETECTION(button) ((inputs.buttons) & (button)) > ((lastInputs.buttons) & (button))
#define FE_DETECTION(button) ((inputs.buttons) & (button)) < ((lastInputs.buttons) & (button))

#define JOYY      BIT(0)
#define JOYX      BIT(1)
#define JOYB      BIT(2)
#define JOYA      BIT(3)
#define JOYSR_R   BIT(4)
#define JOYSL_R   BIT(5)
#define JOYR      BIT(6)
#define JOYZR     BIT(7)

#define JOYMINUS  BIT(8)
#define JOYPLUS   BIT(9)
#define JOYR3     BIT(10)
#define JOYL3     BIT(11)
#define JOYHOME   BIT(12)
#define JOYCAP    BIT(13)
#define JOYPAD    BIT(14)
#define JOYWIRED  BIT(15)

#define JOYLDOWN  BIT(16)
#define JOYLUP    BIT(17)
#define JOYLRIGHT BIT(18)
#define JOYLLEFT  BIT(19)
#define JOYSR_L   BIT(20)
#define JOYSL_L   BIT(21)
#define JOYL      BIT(22)
#define JOYZL     BIT(23)

#define BTNPOW    BIT(24)
#define BTNVOLP   BIT(25)
#define BTNVOLM   BIT(26)

#define JOYRDOWN  BIT(27)
#define JOYRUP    BIT(28)
#define JOYRRIGHT BIT(29)
#define JOYRLEFT  BIT(30)
#define JOYUNUSED BIT(31)

#define WAITBUTTONS (JOYY | JOYX | JOYB | JOYA | JOYLDOWN | JOYLUP | JOYLRIGHT | JOYLLEFT)

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

            // Console
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

extern input_t inputs;
extern input_t lastInputs;

void     hidInit();
input_t *hidRead();
input_t *hidWait();
input_t *hidWaitMask(u32 mask);
bool     hidConnected();

#endif //_HID_H