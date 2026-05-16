//
// Created by auggiethecat on 5/16/26.
//

#ifndef _KEYS_H
#define _KEYS_H

#include <bdk.h>

typedef struct _key_storage_t {
    u8  bis_key[4][SE_KEY_128_SIZE * 2];
    // FS-related keys
    u8  header_key[SE_KEY_128_SIZE * 2];
    u8  save_mac_key[SE_KEY_128_SIZE];
    // master key-derived families
    u8  master_kek[SE_KEY_128_SIZE];

    //TODO: Do I need keyblobs?
    // keyblob_t keyblob[KB_FIRMWARE_VERSION_600 + 1];
} key_storage_t;

#endif //_KEYS_H