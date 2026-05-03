//
// Created by auggiethecat on 5/2/26.
//

#include "vector.h"

#include <bdk.h>
#include <string.h>

void *_vecInit(u32 elementSize, u32 preallocate) {
    u32 capacity        = preallocate ? elementSize * preallocate : elementSize * 1;
    vecHeader_t* header = zalloc(sizeof(vecHeader_t) + capacity);

    header->capacity    = capacity;
    header->count       = 0;
    header->elementSize = elementSize;

    return header + 1;
}

void *_vecFromArray(void *data, u32 count, u32 elementSize) {
    u32 capacity        = count * elementSize;
    vecHeader_t* header = zalloc(sizeof(vecHeader_t) + capacity);

    header->capacity    = capacity;
    header->count       = count;
    header->elementSize = elementSize;

    memcpy(header + 1, data, capacity);
    return header + 1;
}

void *_vecValidateCapacity(void *vec, u32 count, u32 elementSize) {
    vecHeader_t *newVecHeader = vecHeader(vec);
    u32 wantedCapacity = (newVecHeader->count + count) * elementSize;

    if (newVecHeader->capacity < wantedCapacity) {
        u32 newCapacity = newVecHeader->capacity ? newVecHeader->capacity * 2 : wantedCapacity;
        while (newCapacity < wantedCapacity) newCapacity *= 2;

        vecHeader_t *newHeader = zalloc(sizeof(vecHeader_t) + newCapacity);

        if (newHeader) {
            u32 oldSize = sizeof(vecHeader_t) + newVecHeader->count * elementSize;
            memcpy(newHeader, newVecHeader, oldSize);

            free(newVecHeader);

            newHeader->capacity = newCapacity;
            newVecHeader = newHeader + 1;
        } else {
            newVecHeader = 0;
        }
    } else {
        newVecHeader = newVecHeader + 1;
    }

    return newVecHeader;
}
