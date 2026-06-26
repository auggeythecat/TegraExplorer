//
// Created by auggiethecat on 5/16/26.
//

#ifndef _KEYS_H
#define _KEYS_H

#include <bdk.h>

#define KB_FIRMWARE_VERSION_100  0
#define KB_FIRMWARE_VERSION_300  1
#define KB_FIRMWARE_VERSION_301  2
#define KB_FIRMWARE_VERSION_400  3
#define KB_FIRMWARE_VERSION_500  4
#define KB_FIRMWARE_VERSION_600  5
#define KB_FIRMWARE_VERSION_620  6
#define KB_FIRMWARE_VERSION_700  7
#define KB_FIRMWARE_VERSION_810  8

#define KB_FIRMWARE_VERSION_MAX  KB_FIRMWARE_VERSION_810

#define KS_BIS_00_CRYPT  0
#define KS_BIS_00_TWEAK  1
#define KS_BIS_01_CRYPT  2
#define KS_BIS_01_TWEAK  3
#define KS_BIS_02_CRYPT  4
#define KS_BIS_02_TWEAK  5
#define KS_AES_CTR       6

#define KS_AES_ECB       8

#define KS_AES_CMAC      10
#define KS_TSEC_ROOT_DEV 11
#define KS_TSEC          12
#define KS_TSEC_ROOT     13
#define KS_SECURE_BOOT   14

#define SET_SEAL_KEY_INDEX(x)   (((x) & 7) << 5)
#define GET_SEAL_KEY_INDEX(x)   (((x) >> 5) & 7)
#define GET_IS_DEVICE_UNIQUE(x) ((x) & 1)

typedef struct _keyStorage_t {
    u8  tempKey[SE_KEY_128_SIZE];
    u8  bisKeys[4][SE_KEY_128_SIZE * 2];

    u8  masterKey0[SE_KEY_128_SIZE];

    u8  headerKey[SE_KEY_128_SIZE * 2];
    u8  saveMacKey[SE_KEY_128_SIZE];

    u8  tsecKey[SE_KEY_128_SIZE];
    u8  tsecRootKey[SE_KEY_128_SIZE];
    u8  secureBootKey[SE_KEY_128_SIZE];

    u8  keyblobKey[SE_KEY_128_SIZE];
    u8  keyblobMacKey[SE_KEY_128_SIZE];

    u8  deviceKey[SE_KEY_128_SIZE];
    u8  deviceKey4x[SE_KEY_128_SIZE];
} keyStorage_t;

void deriveRelevantKeys(keyStorage_t *keys);

#endif //_KEYS_H