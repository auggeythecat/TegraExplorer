//
// Created by auggiethecat on 5/3/26.
//

#include "mainMenu.h"
#include "../../gfx/menu.h"
#include "../util/error.h"
#include "../util/utils.h"

enum {
    MainExplore = 0,
    MainBrowseSd,
    MainExit,
    MainPowerOff,
    MainRebootRCM,
    MainRebootNormal,
    MainRebootHekate,
};

menuEntry_t mainMenu[] = {
    [MainExplore]      = {.optionUnion = COLORTORGB(COLOR_WHITE) , .name = "-- Explore --"},
    [MainBrowseSd]     = {.optionUnion = COLORTORGB(COLOR_GREEN) , .name = "Browse SD"},
    [MainExit]         = {.optionUnion = COLORTORGB(COLOR_WHITE) , .name = "-- Exit --"},
    [MainPowerOff]     = {.optionUnion = COLORTORGB(COLOR_VIOLET), .name = "Power off"},
    [MainRebootRCM]    = {.optionUnion = COLORTORGB(COLOR_VIOLET), .name = "Reboot to RCM"},
    [MainRebootNormal] = {.optionUnion = COLORTORGB(COLOR_VIOLET), .name = "Reboot normally"},
    [MainRebootHekate] = {.optionUnion = COLORTORGB(COLOR_VIOLET), .name = "Reboot to bootloader/update.bin"}
};


void explore() {
    newError(TE_ERROR_NOT_IMPL_YET);
}

void rebootHekate() {
    launch_payload("/bootloader/update.bin", false);
}

menuPath_t mainMenuPaths[] = {
    [MainBrowseSd]     = explore,
    [MainPowerOff]     = powerOff,
    [MainRebootRCM]    = rebootRCM,
    [MainRebootNormal] = reboot,
    [MainRebootHekate] = rebootHekate,
};

void enterMainMenu() {

}
