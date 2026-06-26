//
// Created by auggiethecat
//

#include <bdk.h>

#include "mountManager.h"

#include "../storage/emummc.h"
#include "../util/config.h"

LIST_INIT(currentGPT);

error_t unmountMMCPartition() {
    int res = 0;

    if (TEConfig.MMCMounted)
        res = f_unmount("bis:");

    TEConfig.MMCMounted = MMC_CONN_None;

    return newError(res);

}

error_t mountMMCPartition(const char* partition) {
    if (!TEConfig.currentMMC)
        return newError(TE_ERROR_PARTITION_NOT_FOUND);

    error_t error = unmountMMCPartition();
    if (error.code)
        return error;

    emmc_part_t *systemPart = emmc_part_find(&currentGPT, partition);
    if (!systemPart)
        return newError(TE_ERROR_PARTITION_NOT_FOUND);

    nx_emmc_bis_init(systemPart, false, TEConfig.currentMMC == MMC_CONN_EMUMMC ? emuCFG.activePart : 0); // TOOD: This can't be right. Look into it better.

    int res = f_mount(&emmc_fs, "bis:", 1);
    return newError(res);
}

error_t disconnectMMC() {
    int res = 0;

    error_t unmountError = unmountMMCPartition();

    if (unmountError.code)
        return unmountError;

    if (TEConfig.currentMMC) {
        TEConfig.currentMMC = MMC_CONN_None;
        res = sdmmc_storage_end(&emmc_storage);
        nx_emmc_bis_end(&currentGPT);
    }

    return newError(res);
}


error_t connectMMC(u8 MMCType) {
    if (MMCType = TEConfig.currentMMC)
        return newError(0);

    disconnectMMC();
    TEConfig.emummcForceDisable = (MMCType == MMC_CONN_EMMC);
    int res = emummcStorageInitMMC(&emmc_storage, &emmc_sdmmc);

    if (res)
        return newError(res);

    TEConfig.currentMMC = MMCType;
    emummcStorageSetMMCPartition(0); // TODO: Why does it do this?
    emmc_gpt_parse(&currentGPT);
    return newError(0);
}

link_t *getCurrentGPT() {
    if (TEConfig.MMCMounted)
        return &currentGPT;

    return NULL;
}
