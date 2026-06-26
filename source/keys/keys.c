//
// Created by auggiethecat on 5/16/26.
//

#include <bdk.h>
#include <string.h>

#include "keys.h"
#include "tsecKeygen.h"
#include "../../util/config.h"
#include "../../util/utils.h"

typedef enum {
    _NOT_DEVICE_UNIQUE = 0,
    _IS_DEVICE_UNIQUE = 1,
} _deviceUnique_t;

int _keyExists(const void *data) { return memcmp(data, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) != 0; }

void _getDeviceKey(u32 ks, keyStorage_t *keys, void *outDeviceKey, u32 generation) {
    if (generation == KB_FIRMWARE_VERSION_100 && !TEConfig.t210b01) {
        memcpy(outDeviceKey, keys->deviceKey, SE_KEY_128_SIZE);
        return;
    }

    if (generation >= KB_FIRMWARE_VERSION_400) {
        generation -= KB_FIRMWARE_VERSION_400;
    } else {
        generation = 0;
    }
    u32 tempKeySource[SE_KEY_128_SIZE / 4] = {0};
    se_aes_key_set(ks, keys->deviceKey4x, SE_KEY_128_SIZE);
    // se_aes_crypt_ecb(ks, DECRYPT, tempKeySource, _deviceMasterKeySourceSources[generation]); // TODO: Are these required? How many?
    // const void *kekSource = fuse_read_hw_state() == FUSE_NX_HW_STATE_PROD ? _deviceMasterKekSources[generation] : _deviceMasterKekSourcesDev[generation];
    // se_aes_key_set(ks, keys->master_key, SE_KEY_128_SIZE);
    // se_aes_unwrap_key(ks, ks, kekSource);
    // se_aes_crypt_ecb(ks, DECRYPT, outDeviceKey, tempKeySource, SE_AES_BLOCK_SIZE);
}

void _getSecureData(keyStorage_t *keys, void *outSata) {
    static const u8 _secureDataSource[0x10] __attribute__((aligned(4))) = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    static u8 _secureDataCounters[0x10] __attribute__((aligned(4))) = {
        0x3C, 0xD5, 0x92, 0xEC, 0x68, 0x31, 0x4A, 0x06, 0xD4, 0x1B, 0x0C, 0xD9, 0xF6, 0x2E, 0xD9, 0xE9
    };

    static const u8 _secureDataTweaks[1][0x10] __attribute__((aligned(4))) = {
        {0xAC, 0xCA, 0x9A, 0xCA, 0xFF, 0x2E, 0xB9, 0x22, 0xCC, 0x1F, 0x4F, 0xAD, 0xDD, 0x77, 0x21, 0x1E}
    };

    se_aes_key_set(KS_AES_CTR, keys->deviceKey, SE_KEY_128_SIZE);
    u8 *d = (u8 *)outSata;
    se_aes_crypt_ctr(KS_AES_CTR, d + SE_KEY_128_SIZE * 0, _secureDataSource, SE_KEY_128_SIZE, _secureDataCounters);
    se_aes_crypt_ctr(KS_AES_CTR, d + SE_KEY_128_SIZE * 1, _secureDataSource, SE_KEY_128_SIZE, _secureDataCounters);

    // Apply tweak
    for (u32 i = 0; i < SE_KEY_128_SIZE; i++) {
        d[SE_KEY_128_SIZE + i] ^= _secureDataTweaks[0][i];
    }
}

void _generateSpecificAESKey(u32 ks, keyStorage_t *keys, void *outKey, const void *keySource, u32 generation) {

    static const u8 _retailSpecificAESKeySource[0x10] __attribute__((aligned(4))) = {
        0xE2, 0xD6, 0xB8, 0x7A, 0x11, 0x9C, 0xB8, 0x80, 0xE8, 0x22, 0x88, 0x8A, 0x46, 0xFB, 0xA1, 0x95};

    if (fuseReadBootromRevision() >= 0x7F) {
        _getDeviceKey(ks, keys, keys->tempKey, generation == 0 ? 0 : generation - 1);
        se_aes_key_set(ks, keys->tempKey, SE_KEY_128_SIZE);
        se_aes_unwrap_key(ks, ks, _retailSpecificAESKeySource);
        se_aes_crypt_ecb(ks, DECRYPT, outKey, keySource, SE_KEY_128_SIZE * 2);
    } else {
        _getSecureData(keys, outKey);
    }
}

void _generateAESKey(u32 ks, keyStorage_t *keys, void *outKey, u32 keySize, const void *accessKey, const void *keySource) {
    static const u8 _aesKeyGenerationSource[0x10] __attribute__((aligned(4))) = {
        0x89, 0x61, 0x5E, 0xE0, 0x5C, 0x31, 0xB6, 0x80, 0x5F, 0xE5, 0x8F, 0x3D, 0xA2, 0x4F, 0x7A, 0xA8};

    u32 AESKey[SE_KEY_128_SIZE / 4] = {0};
    se_aes_key_set(ks, accessKey, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(ks, DECRYPT, AESKey, _aesKeyGenerationSource, SE_AES_BLOCK_SIZE);
    se_aes_key_set(ks, AESKey, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(ks, DECRYPT, outKey, keySource, keySize);
}

void _generateAESKek(u32 ks, keyStorage_t *keys, void *outKek, const void *kekSource, u32 generation, u32 option) {
    static const u8 _AESKekGenerationSource[0x10] __attribute__((aligned(4))) = {
        0x4D, 0x87, 0x09, 0x86, 0xC4, 0x5D, 0x20, 0x72, 0x2F, 0xBA, 0x10, 0x53, 0xDA, 0x92, 0xE8, 0xA9};

    static const u8 _sealKeyMasks[][0x10] __attribute__((aligned(4))) = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // SealKey_LoadAesKey
        {0xA2, 0xAB, 0xBF, 0x9C, 0x92, 0x2F, 0xBB, 0xE3, 0x78, 0x79, 0x9B, 0xC0, 0xCC, 0xEA, 0xA5, 0x74}, // SealKey_DecryptDeviceUniqueData
        {0x57, 0xE2, 0xD9, 0x45, 0xE4, 0x92, 0xF4, 0xFD, 0xC3, 0xF9, 0x86, 0x38, 0x89, 0x78, 0x9F, 0x3C}, // SealKey_ImportLotusKey
        {0xE5, 0x4D, 0x9A, 0x02, 0xF0, 0x4F, 0x5F, 0xA8, 0xAD, 0x76, 0x0A, 0xF6, 0x32, 0x95, 0x59, 0xBB}, // SealKey_ImportEsDeviceKey
        {0x59, 0xD9, 0x31, 0xF4, 0xA7, 0x97, 0xB8, 0x14, 0x40, 0xD6, 0xA2, 0x60, 0x2B, 0xED, 0x15, 0x31}, // SealKey_ReencryptDeviceUniqueData
        {0xFD, 0x6A, 0x25, 0xE5, 0xD8, 0x38, 0x7F, 0x91, 0x49, 0xDA, 0xF8, 0x59, 0xA8, 0x28, 0xE6, 0x75}, // SealKey_ImportSslKey
        {0x89, 0x96, 0x43, 0x9A, 0x7C, 0xD5, 0x59, 0x55, 0x24, 0xD5, 0x24, 0x18, 0xAB, 0x6C, 0x04, 0x61}, // SealKey_ImportEsClientCertKey
    };

    bool deviceUnique = GET_IS_DEVICE_UNIQUE(option);
    u32 sealKeyIndex = GET_SEAL_KEY_INDEX(option);

    if (generation)
        generation--;

    u8 staticSource[SE_KEY_128_SIZE] __attribute__((aligned(4)));
    for (u32 i = 0; i < SE_KEY_128_SIZE; i++)
        staticSource[i] = _AESKekGenerationSource[i] ^ _sealKeyMasks[sealKeyIndex][i];

    if (deviceUnique) {
        _getDeviceKey(ks, keys, keys->tempKey, generation);
    } else {
        memcpy(keys->tempKey, keys->masterKey0, sizeof(keys->tempKey));
    }
    se_aes_key_set(ks, keys->tempKey, SE_KEY_128_SIZE);
    se_aes_unwrap_key(ks, ks, staticSource);
    se_aes_crypt_ecb(ks, DECRYPT, outKek, kekSource, SE_AES_BLOCK_SIZE);
}

void _deriveBisKeys(keyStorage_t *keys, u8 outBisKeys[4][32]) {

    static const u8 _bisKekSource[0x10] __attribute__((aligned(4))) = {
        0x34, 0xC1, 0xA0, 0xC4, 0x82, 0x58, 0xF8, 0xB4, 0xFA, 0x9E, 0x5E, 0x6A, 0xDA, 0xFC, 0x7E, 0x4F};

    static const u8 _bisKeySources[3][0x20] __attribute__((aligned(4))) = {
        {0xF8, 0x3F, 0x38, 0x6E, 0x2C, 0xD2, 0xCA, 0x32, 0xA8, 0x9A, 0xB9, 0xAA, 0x29, 0xBF, 0xC7, 0x48,
         0x7D, 0x92, 0xB0, 0x3A, 0xA8, 0xBF, 0xDE, 0xE1, 0xA7, 0x4C, 0x3B, 0x6E, 0x35, 0xCB, 0x71, 0x06},
        {0x41, 0x00, 0x30, 0x49, 0xDD, 0xCC, 0xC0, 0x65, 0x64, 0x7A, 0x7E, 0xB4, 0x1E, 0xED, 0x9C, 0x5F,
         0x44, 0x42, 0x4E, 0xDA, 0xB4, 0x9D, 0xFC, 0xD9, 0x87, 0x77, 0x24, 0x9A, 0xDC, 0x9F, 0x7C, 0xA4},
        {0x52, 0xC2, 0xE9, 0xEB, 0x09, 0xE3, 0xEE, 0x29, 0x32, 0xA1, 0x0C, 0x1F, 0xB6, 0xA0, 0x92, 0x6C,
         0x4D, 0x12, 0xE1, 0x4B, 0x2A, 0x47, 0x4C, 0x1C, 0x09, 0xCB, 0x03, 0x59, 0xF0, 0x15, 0xF4, 0xE4}
    };

    if ((!TEConfig.t210b01 && !_keyExists(keys->deviceKey)) || (TEConfig.t210b01 && (!_keyExists(keys->masterKey0) || !_keyExists(keys->deviceKey4x)))) {
        return;
    }

    u32 generation = fuse_read_odm_keygen_rev();

    _generateSpecificAESKey(KS_AES_ECB, keys, outBisKeys[0], _bisKeySources[0], generation);
    u32 accessKey[SE_KEY_128_SIZE / 4] = {0};
    const u32 option = _IS_DEVICE_UNIQUE;
    _generateAESKek(KS_AES_ECB, keys, accessKey, _bisKekSource, generation, option);
    _generateAESKey(KS_AES_ECB, keys, outBisKeys[1], sizeof(_bisKeySources[1]), accessKey, _bisKeySources[1]);
    _generateAESKey(KS_AES_ECB, keys, outBisKeys[2], sizeof(_bisKeySources[2]), accessKey, _bisKeySources[2]);
    memcpy(outBisKeys[3], outBisKeys[2], sizeof(_bisKeySources[2]));
}

void _deriveDeviceKeys(keyStorage_t *keys) {
    static const u8 _keyblobKeySource[0x10] __attribute__((aligned(4))) = {
        0xDF, 0x20, 0x6F, 0x59, 0x44, 0x54, 0xEF, 0xDC, 0x70, 0x74, 0x48, 0x3B, 0x0D, 0xED, 0x9F, 0xD3};

    static const u8 _keyblobMacKeySource[0x10] __attribute__((aligned(4))) = {
        0x59, 0xC7, 0xFB, 0x6F, 0xBE, 0x9B, 0xBE, 0x87, 0x65, 0x6B, 0x15, 0xC0, 0x53, 0x73, 0x36, 0xA5};

    static const u8 _perConsoleKeySource[0x10] __attribute__((aligned(4))) = {
        0x4F, 0x02, 0x5F, 0x0E, 0xB6, 0x6D, 0x11, 0x0E, 0xDC, 0x32, 0x7D, 0x41, 0x86, 0xC2, 0xF4, 0x78};

    static const u8 _deviceMasterKeySourceKekSource[0x10] __attribute__((aligned(4))) = {
        0x0C, 0x91, 0x09, 0xDB, 0x93, 0x93, 0x07, 0x81, 0x07, 0x3C, 0xC4, 0x16, 0x22, 0x7C, 0x6C, 0x28};

    se_aes_crypt_ecb(KS_TSEC, DECRYPT, keys->keyblobKey, _keyblobKeySource, SE_AES_BLOCK_SIZE);
    se_aes_crypt_ecb(KS_SECURE_BOOT, DECRYPT, keys->keyblobKey, keys->keyblobKey, SE_AES_BLOCK_SIZE);
    se_aes_key_set(KS_AES_ECB, keys->keyblobKey, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, keys->keyblobMacKey, _keyblobMacKeySource, SE_AES_BLOCK_SIZE);

    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, keys->deviceKey, _perConsoleKeySource, SE_AES_BLOCK_SIZE);
    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, keys->deviceKey4x, _deviceMasterKeySourceKekSource, SE_AES_BLOCK_SIZE);
}

int _runAMSKeygen() {
    tsec_ctxt_t tsec_ctxt;
    tsec_ctxt.fw   = (u8*) tsecKeygen;
    tsec_ctxt.size = sizeof(tsecKeygen);
    tsec_ctxt.type = TSEC_FW_TYPE_NEW;

    u32 retries = 0;
    u32 tempKey[SE_KEY_128_SIZE / 4];
    while (tsec_query(tempKey, &tsec_ctxt) < 0) {
        retries++;
        if (retries > 15) {
            return -1;
        }
    }

    return 0;
}

// static void _deriveMasterKeysFromLatestKey(keyStorage_t *keys, bool is_dev) {
//     if (!TEConfig.t210b01) {
//         u32 tsecRootKeySlot = is_dev ? KS_TSEC_ROOT_DEV : KS_TSEC_ROOT;
//         // Derive all master keys based on current root key
//         for (u32 i = KB_FIRMWARE_VERSION_810 - KB_FIRMWARE_VERSION_620; i < ARRAY_SIZE(master_kek_sources); i++) {
//             u32 key_index = i + KB_FIRMWARE_VERSION_620;
//             se_aes_crypt_ecb(tsec_root_key_slot, DECRYPT, keys->master_kek[key_index], master_kek_sources[i], SE_AES_BLOCK_SIZE);
//             load_aes_key(KS_AES_ECB, keys->master_key[key_index], keys->master_kek[key_index], master_key_source);
//         }
//     }
// }

void _deriveMasterKeys(keyStorage_t *keys, bool isDev) {

    if (TEConfig.t210b01) {
        // _deriveMasterKeysMariko(keys, isDev);
        // _deriveMasterKeysFromLatestKey(keys, isDev);
    } else {
        if (_runAMSKeygen()) {
            EPRINTF("Failed to run keygen.");
            return;
        }
    }

    u8 *aesKeys = (u8 *)calloc(1, SZ_4K);
    se_aes_ctx_get_keys(aesKeys + SZ_2K, aesKeys, SE_KEY_128_SIZE);
    memcpy(&keys->tsecKey,      aesKeys + KS_TSEC              * SE_KEY_128_SIZE, SE_KEY_128_SIZE);
    memcpy(&keys->tsecRootKey, aesKeys + KS_TSEC_ROOT         * SE_KEY_128_SIZE, SE_KEY_128_SIZE);
    if (FUSE(FUSE_PRIVATE_KEY0) != 0xFFFFFFFF) {
        memcpy(&keys->secureBootKey, aesKeys + KS_SECURE_BOOT * SE_KEY_128_SIZE, SE_KEY_128_SIZE);
    }

    free(aesKeys);

    // _deriveMasterKeysFromLatestKey(keys, false);
}

void _deriveHeaderKey(keyStorage_t *keys, void *outKey) {
    static const u8 _headerKekSource[0x10] __attribute__((aligned(4))) = {
        0x1F, 0x12, 0x91, 0x3A, 0x4A, 0xCB, 0xF0, 0x0D, 0x4C, 0xDE, 0x3A, 0xF6, 0xD5, 0x23, 0x88, 0x2A};
    static const u8 _headerKeySource[0x20] __attribute__((aligned(4))) = {
        0x5A, 0x3E, 0xD8, 0x4F, 0xDE, 0xC0, 0xD8, 0x26, 0x31, 0xF7, 0xE2, 0x5D, 0x19, 0x7B, 0xF5, 0xD0,
        0x1C, 0x9B, 0x7B, 0xFA, 0xF6, 0x28, 0x18, 0x3D, 0x71, 0xF6, 0x4D, 0x73, 0xF1, 0x50, 0xB9, 0xD2};

    if (!_keyExists(keys->masterKey0)) {
        return;
    }

    u32 accessKey[SE_KEY_128_SIZE / 4] = {0};
    const u32 generation = 0;
    const u32 option = _NOT_DEVICE_UNIQUE;
    _generateAESKek(KS_AES_ECB, keys, accessKey, _headerKekSource, generation, option);
    _generateAESKey(KS_AES_ECB, keys, outKey, sizeof(_headerKeySource), accessKey, _headerKeySource);
}

void _deriveSaveMacKey(keyStorage_t *keys, void *out_key) {
    static const u8 _saveMacKekSource[0x10] __attribute__((aligned(4))) = {
        0xD8, 0x9C, 0x23, 0x6E, 0xC9, 0x12, 0x4E, 0x43, 0xC8, 0x2B, 0x03, 0x87, 0x43, 0xF9, 0xCF, 0x1B};
    static const u8 _saveMacKeySource[0x10] __attribute__((aligned(4))) = {
        0xE4, 0xCD, 0x3D, 0x4A, 0xD5, 0x0F, 0x74, 0x28, 0x45, 0xA4, 0x87, 0xE5, 0xA0, 0x63, 0xEA, 0x1F};

    if ((!TEConfig.t210b01 && !_keyExists(keys->deviceKey)) || (TEConfig.t210b01 && (!_keyExists(keys->masterKey0) || !_keyExists(keys->deviceKey4x)))) {
        return;
    }

    u32 accessKey[SE_KEY_128_SIZE / 4] = {0};
    const u32 generation = 0;
    const u32 option = _IS_DEVICE_UNIQUE;
    _generateAESKek(KS_AES_ECB, keys, accessKey, _saveMacKekSource, generation, option);
    se_aes_key_set(KS_AES_ECB, accessKey, SE_KEY_128_SIZE);
    se_aes_crypt_ecb(KS_AES_ECB, DECRYPT, out_key, _saveMacKeySource, SE_AES_BLOCK_SIZE);
}

void deriveRelevantKeys(keyStorage_t *keys) {
    _deriveMasterKeys(keys, fuse_read_hw_state() == FUSE_NX_HW_STATE_DEV);
    _deriveDeviceKeys(keys);
    _deriveBisKeys(     keys, keys->bisKeys);
    _deriveHeaderKey(keys, keys->headerKey);
    _deriveSaveMacKey(keys, keys->saveMacKey);
}