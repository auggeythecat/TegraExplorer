//
// Created by auggiethecat on 5/16/26.
//

#include <bdk.h>
#include <string.h>

#include "keys.h"
#include "tsec_keygen.h"
#include "../../util/config.h"
#include "../../util/utils.h"

typedef enum {
    _NOT_DEVICE_UNIQUE = 0,
    _IS_DEVICE_UNIQUE = 1,
} _device_unique_t;

int _key_exists(const void *data) { return memcmp(data, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) != 0; }

void _get_device_key(u32 ks, key_storage_t *keys, void *out_device_key, u32 generation) {
    if (generation == KB_FIRMWARE_VERSION_100 && !TEConfig.t210b01) {
        memcpy(out_device_key, keys->device_key, SE_KEY_128_SIZE);
        return;
    }

    if (generation >= KB_FIRMWARE_VERSION_400) {
        generation -= KB_FIRMWARE_VERSION_400;
    } else {
        generation = 0;
    }
    u32 temp_key_source[SE_KEY_128_SIZE / 4] = {0};
    se_aes_key_set(ks, keys->device_key_4x, SE_KEY_128_SIZE);
    // se_aes_crypt_ecb(ks, DECRYPT, temp_key_source, _device_master_key_source_sources[generation]); // TODO: Are these required? How many?
    // const void *kek_source = fuse_read_hw_state() == FUSE_NX_HW_STATE_PROD ? _device_master_kek_sources[generation] : _device_master_kek_sources_dev[generation];
    // se_aes_key_set(ks, keys->master_key, SE_KEY_128_SIZE);
    // se_aes_unwrap_key(ks, ks, kek_source);
    // se_aes_crypt_ecb(ks, DECRYPT, out_device_key, temp_key_source, SE_AES_BLOCK_SIZE);
}

void _get_secure_data(key_storage_t *keys, void *out_data) {
    static const u8 _secure_data_source[0x10] __attribute__((aligned(4))) = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    static const u8 _secure_data_counters[1][0x10] __attribute__((aligned(4))) = {
        {0x3C, 0xD5, 0x92, 0xEC, 0x68, 0x31, 0x4A, 0x06, 0xD4, 0x1B, 0x0C, 0xD9, 0xF6, 0x2E, 0xD9, 0xE9}
    };

    static const u8 _secure_data_tweaks[1][0x10] __attribute__((aligned(4))) = {
        {0xAC, 0xCA, 0x9A, 0xCA, 0xFF, 0x2E, 0xB9, 0x22, 0xCC, 0x1F, 0x4F, 0xAD, 0xDD, 0x77, 0x21, 0x1E}
    };

    se_aes_key_set(KS_AES_CTR, keys->device_key, SE_KEY_128_SIZE);
    u8 *d = (u8 *)out_data;
    se_aes_crypt_ctr(KS_AES_CTR, d + SE_KEY_128_SIZE * 0, _secure_data_source, SE_KEY_128_SIZE, _secure_data_counters[0]);
    se_aes_crypt_ctr(KS_AES_CTR, d + SE_KEY_128_SIZE * 1, _secure_data_source, SE_KEY_128_SIZE, _secure_data_counters[0]);

    // Apply tweak
    for (u32 i = 0; i < SE_KEY_128_SIZE; i++) {
        d[SE_KEY_128_SIZE + i] ^= _secure_data_tweaks[0][i];
    }
}

void _generate_specific_aes_key(u32 ks, key_storage_t *keys, void *out_key, const void *key_source, u32 generation) {

    static const u8 _retail_specific_aes_key_source[0x10] __attribute__((aligned(4))) = {
        0xE2, 0xD6, 0xB8, 0x7A, 0x11, 0x9C, 0xB8, 0x80, 0xE8, 0x22, 0x88, 0x8A, 0x46, 0xFB, 0xA1, 0x95};

    if (fuse_read_bootrom_rev() >= 0x7F) {
        _get_device_key(ks, keys, keys->temp_key, generation == 0 ? 0 : generation - 1);
        se_aes_key_set(ks, keys->temp_key, SE_KEY_128_SIZE);
        se_aes_unwrap_key(ks, ks, _retail_specific_aes_key_source);
        se_aes_crypt_ecb(ks, DECRYPT, out_key, key_source, SE_KEY_128_SIZE * 2);
    } else {
        _get_secure_data(keys, out_key);
    }
}

void _generate_aes_key(u32 ks, key_storage_t *keys, void *out_key, u32 key_size, const void *access_key, const void *key_source) {
    static const u8 _aes_key_generation_source[0x10] __attribute__((aligned(4))) = {
        0x89, 0x61, 0x5E, 0xE0, 0x5C, 0x31, 0xB6, 0x80, 0x5F, 0xE5, 0x8F, 0x3D, 0xA2, 0x4F, 0x7A, 0xA8};

    u32 aes_key[SE_KEY_128_SIZE / 4] = {0};
    se_aes_key_set(ks, access_key, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(ks, DECRYPT, aes_key, _aes_key_generation_source, SE_AES_BLOCK_SIZE);
    se_aes_key_set(ks, aes_key, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(ks, DECRYPT, out_key, key_source, key_size);
}

void _generate_aes_kek(u32 ks, key_storage_t *keys, void *out_kek, const void *kek_source, u32 generation, u32 option) {
    static const u8 _aes_kek_generation_source[0x10] __attribute__((aligned(4))) = {
        0x4D, 0x87, 0x09, 0x86, 0xC4, 0x5D, 0x20, 0x72, 0x2F, 0xBA, 0x10, 0x53, 0xDA, 0x92, 0xE8, 0xA9};

    static const u8 _seal_key_masks[][0x10] __attribute__((aligned(4))) = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // SealKey_LoadAesKey
        {0xA2, 0xAB, 0xBF, 0x9C, 0x92, 0x2F, 0xBB, 0xE3, 0x78, 0x79, 0x9B, 0xC0, 0xCC, 0xEA, 0xA5, 0x74}, // SealKey_DecryptDeviceUniqueData
        {0x57, 0xE2, 0xD9, 0x45, 0xE4, 0x92, 0xF4, 0xFD, 0xC3, 0xF9, 0x86, 0x38, 0x89, 0x78, 0x9F, 0x3C}, // SealKey_ImportLotusKey
        {0xE5, 0x4D, 0x9A, 0x02, 0xF0, 0x4F, 0x5F, 0xA8, 0xAD, 0x76, 0x0A, 0xF6, 0x32, 0x95, 0x59, 0xBB}, // SealKey_ImportEsDeviceKey
        {0x59, 0xD9, 0x31, 0xF4, 0xA7, 0x97, 0xB8, 0x14, 0x40, 0xD6, 0xA2, 0x60, 0x2B, 0xED, 0x15, 0x31}, // SealKey_ReencryptDeviceUniqueData
        {0xFD, 0x6A, 0x25, 0xE5, 0xD8, 0x38, 0x7F, 0x91, 0x49, 0xDA, 0xF8, 0x59, 0xA8, 0x28, 0xE6, 0x75}, // SealKey_ImportSslKey
        {0x89, 0x96, 0x43, 0x9A, 0x7C, 0xD5, 0x59, 0x55, 0x24, 0xD5, 0x24, 0x18, 0xAB, 0x6C, 0x04, 0x61}, // SealKey_ImportEsClientCertKey
    };

    bool device_unique = GET_IS_DEVICE_UNIQUE(option);
    u32 seal_key_index = GET_SEAL_KEY_INDEX(option);

    if (generation)
        generation--;

    u8 static_source[SE_KEY_128_SIZE] __attribute__((aligned(4)));
    for (u32 i = 0; i < SE_KEY_128_SIZE; i++)
        static_source[i] = _aes_kek_generation_source[i] ^ _seal_key_masks[seal_key_index][i];

    if (device_unique) {
        _get_device_key(ks, keys, keys->temp_key, generation);
    } else {
        memcpy(keys->temp_key, keys->master_key, sizeof(keys->temp_key));
    }
    se_aes_key_set(ks, keys->temp_key, SE_KEY_128_SIZE);
    se_aes_unwrap_key(ks, ks, static_source);
    se_aes_crypt_ecb(ks, DECRYPT, out_kek, kek_source, SE_AES_BLOCK_SIZE);
}

void _derive_bis_keys(key_storage_t *keys, u8 out_bis_keys[4][32]) {

    static const u8 _bis_kek_source[0x10] __attribute__((aligned(4))) = {
        0x34, 0xC1, 0xA0, 0xC4, 0x82, 0x58, 0xF8, 0xB4, 0xFA, 0x9E, 0x5E, 0x6A, 0xDA, 0xFC, 0x7E, 0x4F};

    static const u8 _bis_key_sources[3][0x20] __attribute__((aligned(4))) = {
        {0xF8, 0x3F, 0x38, 0x6E, 0x2C, 0xD2, 0xCA, 0x32, 0xA8, 0x9A, 0xB9, 0xAA, 0x29, 0xBF, 0xC7, 0x48,
         0x7D, 0x92, 0xB0, 0x3A, 0xA8, 0xBF, 0xDE, 0xE1, 0xA7, 0x4C, 0x3B, 0x6E, 0x35, 0xCB, 0x71, 0x06},
        {0x41, 0x00, 0x30, 0x49, 0xDD, 0xCC, 0xC0, 0x65, 0x64, 0x7A, 0x7E, 0xB4, 0x1E, 0xED, 0x9C, 0x5F,
         0x44, 0x42, 0x4E, 0xDA, 0xB4, 0x9D, 0xFC, 0xD9, 0x87, 0x77, 0x24, 0x9A, 0xDC, 0x9F, 0x7C, 0xA4},
        {0x52, 0xC2, 0xE9, 0xEB, 0x09, 0xE3, 0xEE, 0x29, 0x32, 0xA1, 0x0C, 0x1F, 0xB6, 0xA0, 0x92, 0x6C,
         0x4D, 0x12, 0xE1, 0x4B, 0x2A, 0x47, 0x4C, 0x1C, 0x09, 0xCB, 0x03, 0x59, 0xF0, 0x15, 0xF4, 0xE4}
    };

    if ((!TEConfig.t210b01 && !_key_exists(keys->device_key)) || (TEConfig.t210b01 && (!_key_exists(keys->master_key) || !_key_exists(keys->device_key_4x)))) {
        return;
    }

    u32 generation = fuse_read_odm_keygen_rev();

    _generate_specific_aes_key(KS_AES_ECB, keys, out_bis_keys[0], _bis_key_sources[0], generation);
    u32 access_key[SE_KEY_128_SIZE / 4] = {0};
    const u32 option = _IS_DEVICE_UNIQUE;
    _generate_aes_kek(KS_AES_ECB, keys, access_key, _bis_kek_source, generation, option);
    _generate_aes_key(KS_AES_ECB, keys, out_bis_keys[1], sizeof(_bis_key_sources[1]), access_key, _bis_key_sources[1]);
    _generate_aes_key(KS_AES_ECB, keys, out_bis_keys[2], sizeof(_bis_key_sources[2]), access_key, _bis_key_sources[2]);
    memcpy(out_bis_keys[3], out_bis_keys[2], sizeof(_bis_key_sources[2]));
}

void _derive_device_keys(key_storage_t *keys) {
    static const u8 _keyblob_key_source[0x10] __attribute__((aligned(4))) = {
        0xDF, 0x20, 0x6F, 0x59, 0x44, 0x54, 0xEF, 0xDC, 0x70, 0x74, 0x48, 0x3B, 0x0D, 0xED, 0x9F, 0xD3};

    static const u8 _keyblob_mac_key_source[0x10] __attribute__((aligned(4))) = {
        0x59, 0xC7, 0xFB, 0x6F, 0xBE, 0x9B, 0xBE, 0x87, 0x65, 0x6B, 0x15, 0xC0, 0x53, 0x73, 0x36, 0xA5};

    static const u8 _per_console_key_source[0x10] __attribute__((aligned(4))) = {
        0x4F, 0x02, 0x5F, 0x0E, 0xB6, 0x6D, 0x11, 0x0E, 0xDC, 0x32, 0x7D, 0x41, 0x86, 0xC2, 0xF4, 0x78};

    static const u8 _device_master_key_source_kek_source[0x10] __attribute__((aligned(4))) = {
        0x0C, 0x91, 0x09, 0xDB, 0x93, 0x93, 0x07, 0x81, 0x07, 0x3C, 0xC4, 0x16, 0x22, 0x7C, 0x6C, 0x28};

    se_aes_crypt_ecb(KS_TSEC, DECRYPT, keys->keyblob_key, _keyblob_key_source, SE_AES_BLOCK_SIZE);
    se_aes_crypt_ecb(KS_SECURE_BOOT, DECRYPT, keys->keyblob_key, keys->keyblob_key, SE_AES_BLOCK_SIZE);
    se_aes_key_set(KS_AES_ECB, keys->keyblob_key, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, keys->keyblob_mac_key, _keyblob_mac_key_source, SE_AES_BLOCK_SIZE);

    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, keys->device_key, _per_console_key_source, SE_AES_BLOCK_SIZE);
    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, keys->device_key_4x, _device_master_key_source_kek_source, SE_AES_BLOCK_SIZE);
}

int _run_ams_keygen() {
    tsec_ctxt_t tsec_ctxt;
    tsec_ctxt.fw = tsec_keygen;
    tsec_ctxt.size = sizeof(tsec_keygen);
    tsec_ctxt.type = TSEC_FW_TYPE_NEW;

    u32 retries = 0;
    u32 temp_key[SE_KEY_128_SIZE / 4];
    while (tsec_query(temp_key, &tsec_ctxt) < 0) {
        retries++;
        if (retries > 15) {
            return -1;
        }
    }

    return 0;
}

// static void _derive_master_keys_from_latest_key(key_storage_t *keys, bool is_dev) {
//     if (!TEConfig.t210b01) {
//         u32 tsec_root_key_slot = is_dev ? KS_TSEC_ROOT_DEV : KS_TSEC_ROOT;
//         // Derive all master keys based on current root key
//         for (u32 i = KB_FIRMWARE_VERSION_810 - KB_FIRMWARE_VERSION_620; i < ARRAY_SIZE(master_kek_sources); i++) {
//             u32 key_index = i + KB_FIRMWARE_VERSION_620;
//             se_aes_crypt_ecb(tsec_root_key_slot, DECRYPT, keys->master_kek[key_index], master_kek_sources[i], SE_AES_BLOCK_SIZE);
//             load_aes_key(KS_AES_ECB, keys->master_key[key_index], keys->master_kek[key_index], master_key_source);
//         }
//     }
// }

void _derive_master_keys(key_storage_t *keys, bool is_dev) {

    if (TEConfig.t210b01) {
        // _derive_master_keys_mariko(keys, is_dev);
        // _derive_master_keys_from_latest_key(keys, is_dev);
    } else {
        if (_run_ams_keygen()) {
            EPRINTF("Failed to run keygen.");
            return;
        }
    }

    u8 *aes_keys = (u8 *)calloc(1, SZ_4K);
    se_aes_ctx_get_keys(aes_keys + SZ_2K, aes_keys, SE_KEY_128_SIZE);
    memcpy(&keys->tsec_key,      aes_keys + KS_TSEC          * SE_KEY_128_SIZE, SE_KEY_128_SIZE);
    memcpy(&keys->tsec_root_key, aes_keys + KS_TSEC_ROOT     * SE_KEY_128_SIZE, SE_KEY_128_SIZE);
    if (FUSE(FUSE_PRIVATE_KEY0) != 0xFFFFFFFF) {
        memcpy(&keys->secure_boot_key, aes_keys + KS_SECURE_BOOT * SE_KEY_128_SIZE, SE_KEY_128_SIZE);
    }

    free(aes_keys);

    // _derive_master_keys_from_latest_key(keys, false);
}

void _derive_header_key(key_storage_t *keys, void *out_key) {
    static const u8 _header_kek_source[0x10] __attribute__((aligned(4))) = {
        0x1F, 0x12, 0x91, 0x3A, 0x4A, 0xCB, 0xF0, 0x0D, 0x4C, 0xDE, 0x3A, 0xF6, 0xD5, 0x23, 0x88, 0x2A};
    static const u8 _header_key_source[0x20] __attribute__((aligned(4))) = {
        0x5A, 0x3E, 0xD8, 0x4F, 0xDE, 0xC0, 0xD8, 0x26, 0x31, 0xF7, 0xE2, 0x5D, 0x19, 0x7B, 0xF5, 0xD0,
        0x1C, 0x9B, 0x7B, 0xFA, 0xF6, 0x28, 0x18, 0x3D, 0x71, 0xF6, 0x4D, 0x73, 0xF1, 0x50, 0xB9, 0xD2};

    if (!_key_exists(keys->master_key)) {
        return;
    }

    u32 access_key[SE_KEY_128_SIZE / 4] = {0};
    const u32 generation = 0;
    const u32 option = _NOT_DEVICE_UNIQUE;
    _generate_aes_kek(KS_AES_ECB, keys, access_key, _header_kek_source, generation, option);
    _generate_aes_key(KS_AES_ECB, keys, out_key, sizeof(_header_key_source), access_key, _header_key_source);
}

void _derive_save_mac_key(key_storage_t *keys, void *out_key) {
    static const u8 _save_mac_kek_source[0x10] __attribute__((aligned(4))) = {
        0xD8, 0x9C, 0x23, 0x6E, 0xC9, 0x12, 0x4E, 0x43, 0xC8, 0x2B, 0x03, 0x87, 0x43, 0xF9, 0xCF, 0x1B};
    static const u8 _save_mac_key_source[0x10] __attribute__((aligned(4))) = {
        0xE4, 0xCD, 0x3D, 0x4A, 0xD5, 0x0F, 0x74, 0x28, 0x45, 0xA4, 0x87, 0xE5, 0xA0, 0x63, 0xEA, 0x1F};

    if ((!TEConfig.t210b01 && !_key_exists(keys->device_key)) || (TEConfig.t210b01 && (!_key_exists(keys->master_key) || !_key_exists(keys->device_key_4x)))) {
        return;
    }

    u32 access_key[SE_KEY_128_SIZE / 4] = {0};
    const u32 generation = 0;
    const u32 option = _IS_DEVICE_UNIQUE;
    _generate_aes_kek(KS_AES_ECB, keys, access_key, _save_mac_kek_source, generation, option);
    se_aes_key_set(KS_AES_ECB, access_key, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, out_key, _save_mac_key_source, SE_AES_BLOCK_SIZE);
}

void derive_relevant_keys(key_storage_t *keys) {
    _derive_master_keys(keys, fuse_read_hw_state() == FUSE_NX_HW_STATE_DEV);
    _derive_device_keys(keys);
    _derive_bis_keys(keys, keys->bis_keys);
    _derive_header_key(keys, keys->header_key);
    _derive_save_mac_key(keys, keys->save_mac_key);
}