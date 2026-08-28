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
#include "hid.h"

#include <input/joycon.h>

#include "utils/btn.h"

input_t inputs     = {0};
input_t lastInputs = {0};

static u16 LbaseX = 0;
static u16 LbaseY = 0;
static u16 RbaseX = 0;
static u16 RbaseY = 0;

input_t *hidRead() {
    lastInputs = inputs;
    const jc_gamepad_rpt_t *controller = joycon_poll();

    inputs.buttons = 0;

    if (controller != NULL) {
        inputs.buttons = controller->buttons;

        if (controller->conn_l) {
            if ((LbaseX == 0 || LbaseY == 0) || controller->l3){
                LbaseX = controller->lstick_x;
                LbaseY = controller->lstick_y;
            }

            inputs.up    = (controller->up    || (controller->lstick_y > LbaseY + JOYSTICK_THRESHOLD)) ? 1 : 0;
            inputs.down  = (controller->down  || (controller->lstick_y < LbaseY - JOYSTICK_THRESHOLD)) ? 1 : 0;
            inputs.left  = (controller->left  || (controller->lstick_x < LbaseX - JOYSTICK_THRESHOLD)) ? 1 : 0;
            inputs.right = (controller->right || (controller->lstick_x > LbaseX + JOYSTICK_THRESHOLD)) ? 1 : 0;
        }

        if (controller->conn_r) {
            if ((RbaseX == 0 || RbaseY == 0) || controller->r3){
                RbaseX = controller->rstick_x;
                RbaseY = controller->rstick_y;
            }

            inputs.rUp    = (controller->rstick_y > RbaseY + JOYSTICK_THRESHOLD) ? 1 : 0;
            inputs.rDown  = (controller->rstick_y < RbaseY - JOYSTICK_THRESHOLD) ? 1 : 0;
            inputs.rLeft  = (controller->rstick_x < RbaseX - JOYSTICK_THRESHOLD) ? 1 : 0;
            inputs.rRight = (controller->rstick_x > RbaseX + JOYSTICK_THRESHOLD) ? 1 : 0;
        }
    }

    const u8 btn = btn_read();
    inputs.volUp   = (btn & BTN_VOL_UP)   ? 1 : 0 ;
    inputs.volDown = (btn & BTN_VOL_DOWN) ? 1 : 0 ;
    inputs.power   = (btn & BTN_POWER)    ? 1 : 0 ;
    inputs.up      = inputs.up   || inputs.volUp  ;
    inputs.down    = inputs.down || inputs.volDown;
    inputs.a       = inputs.a    || inputs.power  ;

    return &inputs;
}

void hidInit() {
    jc_init_hw();
    joycon_poll();
}

input_t *hidWaitMask(const u32 mask) {
    input_t *in = hidRead();

    while (in->buttons & mask)
        in = hidRead();

    while (!(in->buttons & mask))
        in = hidRead();

    return in;
}

input_t *hidWait() {
    input_t *in = hidRead();

    while (in->buttons)
        in = hidRead();

    while (!in->buttons)
        in = hidRead();

    return in;
}

bool hidConnected() {
    const jc_gamepad_rpt_t *controller = joycon_poll();
    return (controller->conn_l && controller->conn_r) ? true : false;
}