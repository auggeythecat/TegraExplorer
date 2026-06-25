//
// Created by auggiethecat on 5/3/26.
//

#include "hid.h"
#include "config.h"

input_t inputs = {0};
u16 LbaseX = 0, LbaseY = 0, RbaseX = 0, RbaseY = 0;

void hidInit(){
    jc_init_hw();
    joycon_poll();
}

input_t *hidRead(){
    jc_gamepad_rpt_t *controller = joycon_poll();

    inputs.buttons = 0;

    if (controller != NULL){
        inputs.buttons = controller->buttons;

        if (controller->conn_l) {
            if ((LbaseX == 0 || LbaseY == 0) || controller->l3){
                LbaseX = controller->lstick_x;
                LbaseY = controller->lstick_y;
            }

            inputs.up    = (controller->up    || inputs.volp || (controller->lstick_y > LbaseY + 500)) ? 1 : 0;
            inputs.down  = (controller->down  || inputs.volm || (controller->lstick_y < LbaseY - 500)) ? 1 : 0;
            inputs.left  = (controller->left  || (controller->lstick_x < LbaseX - 500))                ? 1 : 0;
            inputs.right = (controller->right || (controller->lstick_x > LbaseX + 500))                ? 1 : 0;
        } else {
            inputs.up    = inputs.volp;
            inputs.down  = inputs.volm;
        }

        if (controller->conn_r) {
            if ((RbaseX == 0 || RbaseY == 0) || controller->r3){
                RbaseX = controller->rstick_x;
                RbaseY = controller->rstick_y;
            }

            inputs.rUp    = (controller->rstick_y > RbaseY + 500) ? 1 : 0;
            inputs.rDown  = (controller->rstick_y < RbaseY - 500) ? 1 : 0;
            inputs.rLeft  = (controller->rstick_x < RbaseX - 500) ? 1 : 0;
            inputs.rRight = (controller->rstick_x > RbaseX + 500) ? 1 : 0;
        }
    }

    u8 btn = btn_read();
    inputs.volp  = (btn & BTN_VOL_UP)   ? 1 : 0;
    inputs.volm  = (btn & BTN_VOL_DOWN) ? 1 : 0;
    inputs.power = (btn & BTN_POWER)    ? 1 : 0;

    inputs.a = inputs.a || inputs.power;

    return &inputs;
}

input_t *hidWaitMask(u32 mask){
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
    jc_gamepad_rpt_t *controller = joycon_poll();
    return (controller->conn_l && controller->conn_r) ? true : false;
}