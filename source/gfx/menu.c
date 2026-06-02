//
// Created by yes on 5/20/26.
//

#include "menu.h"
#include "../../util/vector.h"
#include "../../util/hid.h"

#include <bdk.h>
#include <string.h>

const char *sizeDefs[] = {
    "B ",
    "KB",
    "MB",
    "GB"
};

void _printEntry(menuEntry_t entry, u32 maxLen, u8 highlighted, u32 bg){
    if (entry.hide)
        return;

    (highlighted) ? SETCOLOR(bg, RGBUNIONTOCOLOR(entry.optionUnion)) : SETCOLOR(RGBUNIONTOCOLOR(entry.optionUnion), bg);

    if (entry.icon){
        gfx_putc(entry.icon);
        gfx_putc(' ');
        maxLen -= 2;
    }

    u32 curX = 0, curY = 0;
    gfx_con_getpos(&curX, &curY);
    gfx_puts_limit(entry.name, maxLen - ((entry.showSize) ? 8 : 0));
    if (entry.showSize){
        (highlighted) ? SETCOLOR(bg, COLOR_BLUE) : SETCOLOR(COLOR_BLUE, bg);
        gfx_con_setpos(curX + (maxLen - 6) * 16, curY);
        gfx_printf("%4d", entry.size);
        gfx_puts_small(sizeDefs[entry.sizeDef]);
    }

    gfx_putc('\n');
}

void _printTopInfo() {
    int battery = 0;
    max17050_get_property(MAX17050_RepSOC, &battery);

    int current_charge_status = 0;
    bq24193_get_property(BQ24193_ChargeStatus, &current_charge_status);
    SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
    gfx_con_setpos(0, 0);
    gfx_printf("Tegraexplorer %d.%d.%d | Battery: %d%% %c\n", TE_VER_MJ, TE_VER_MN, TE_VER_HF, battery >> 8, current_charge_status ? 129 : 32);
    RESETCOLOR;
}


u32 newMenu(vector_t vec, int startIndex, int screenLenX, int screenLenY, u8 options,int entryCount) {
    menuEntry_t *entries = vec;
    u32 selected = startIndex;
    u32 menuItems = vecGetCount(vec);
    u32 fontSize = gfx_con.fntsz;

    while (entries[selected].skip || entries[selected].hide) {
        selected++;
        if (selected > menuItems)
            selected = 0;
    }

    u32 lastIndex = selected;
    u32 startX = 0, startY = 0;
    gfx_con_getpos(&startX, &startY);

    u32 bgColor = COLOR_DEFAULT; // TODO: User control, settings or nyx.ini?

    bool redrawScreen = true;
    input_t *input = hidRead();

    // Maybe add a check here so you don't read OOB by providing a too high startindex?
    // TODO: Not magic numbers ;(
    u32 lastPress = 0x666 + get_tmr_ms();
    u32 holdTimer = 300;

    while (true) {
        u32 lastDraw = get_tmr_us();
        if (redrawScreen || options & ALWAYSREDRAW) {
            if (options & ENABLEPAGECOUNT) {
                SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
                char temp[40] = "";
                s_printf(temp, " Page %d / %d | Total %d entries", (selected / screenLenY) + 1, (menuItems - 1  / screenLenY) + 1, entryCount);
                gfx_con_setpos(1280 - strlen(temp) * 18, 0);
                gfx_printf(temp);
            }


            gfx_con_setpos(startX, startY);

            if (redrawScreen)
                gfx_boxGrey(startX,startY, startX + screenLenX * 16, startY + screenLenY * 16, 0x1b);

            int start = selected / screenLenY * screenLenY;
            gfx_con_setpos(startX, startY);
            gfx_printf("%N", startX);
            for (int i = start; i < MIN(vecGetCount(vec), start + screenLenY); i++){
                _printEntry(entries[i], screenLenX, (i == selected), bgColor);
            }
            gfx_printf("%N", 0);
        } else if (lastIndex != selected) {
            u32 minLastCur = MIN(lastIndex, selected);
            u32 maxLastCur = MAX(lastIndex, selected);
            gfx_con_setpos(startX, startY + ((minLastCur % screenLenY) * 16));
            _printEntry(entries[minLastCur], screenLenX, (minLastCur == selected), bgColor);
            gfx_con_setpos(startX, startY + ((maxLastCur % screenLenY) * 16));
            _printEntry(entries[maxLastCur], screenLenX, (minLastCur != selected), bgColor);
        }
        lastIndex = selected;

        SETCOLOR(COLOR_DEFAULT, COLOR_WHITE);
        gfx_con_setpos(0, 704);
        gfx_printf("Time taken for screen draw: %dms  ", get_tmr_ms() - lastDraw);

        while(hidRead()){
            if (!(input->buttons)){
                holdTimer = 300;
                break;
            }

            if (input->buttons & (JoyRUp | JoyRDown))
                holdTimer = 35;

            if ((lastPress + holdTimer) < get_tmr_ms()){
                if (holdTimer > 50)
                    holdTimer -= 50;
                break;
            }
        }

        while (1){
            if (hidRead()->a)
                return selected;
            else if (input->b && options & ENABLEB)
                return 0;
            else if (input->down || input->rDown || input->right){ //Rdown should probs not trigger a page change. Same for RUp
                u32 temp = (input->right && !(input->down || input->rDown)) ? screenLenY : 1;

                if (vecGetCount(vec) > selected + temp){
                    selected += temp;
                    break;
                }
                else if (input->right && (selected / screenLenY != (vecGetCount(vec) - 1) / screenLenY)){
                    selected = vecGetCount(vec) - 1;
                    break;
                }
            }
            else if (input->up || input->rUp || input->left){
                u32 temp = (input->left && !(input->up || input->rUp)) ? screenLenY : 1;
                if (selected >= temp){
                    selected -= temp;
                    break;
                }
            }
            else if (input->plus) {
                gfx_con_set_fontsz(fontSize += 2);
                redrawScreen = true;
            }
            else if (input->minus) {
                gfx_con_set_fontsz(fontSize -= 2);
                redrawScreen = true;
            }

            else {
                holdTimer = 300;
                _printTopInfo();
            }
        }

        lastPress = get_tmr_ms();

        int m = (selected > lastIndex) ? 1 : -1;
        while (selected > 0 && selected < vecGetCount(vec) - 1 && entries[selected].optionUnion & SKIPHIDEBITS)
            selected += m;

        if (entries[selected].optionUnion & SKIPHIDEBITS)
            selected = lastIndex;

        redrawScreen = (selected / screenLenY != lastIndex / screenLenY);
    }
}