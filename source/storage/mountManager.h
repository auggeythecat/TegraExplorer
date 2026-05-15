//
// Created by auggiethecat
//

#ifndef _MOUNTMANAGER_H
#define _MOUNTMANAGER_H

#include <bdk.h>

#include "../util/error.h"

enum {
    MMC_CONN_None = 0,
    MMC_CONN_EMMC,
    MMC_CONN_EMUMMC
};

error_t connectMMC(u8 MMCType);
error_t disconnectMMC();
error_t mountMMCPartition(const char* partition);
error_t unmountMMCPartition();
link_t *getCurrentGPT();
// void    setKeySlots();

#endif //_MOUNTMANAGER_H
