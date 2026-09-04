/*—————————————————————————————————————————————————————————————————————————————
 — Copyright (c) 2026 auggiethecat!                                           —
 —                                                                            —
 — All code was handwritten without the assistance of a Large Language Model. —
 —                                                                            —
 — This program is free software; you can redistribute it and/or modify it    —
 — under the terms and conditions of the GNU General Public License,          —
 — version 2, as published by the Free Software Foundation.                   —
 —                                                                            —
 — This program is distributed in the hope it will be useful, but WITHOUT     —
 — ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      —
 — FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for  —
 — more details.                                                              —
 —                                                                            —
 — You should have received a copy of the GNU General Public License          —
 — along with this program.  If not, see <http://www.gnu.org/licenses/>.      —
 —————————————————————————————————————————————————————————————————————————————*/

#include "vector.h"

#include <string.h>

#include "mem/heap.h"

void *_vecInit(const u32 elementSize, const u32 preallocate) {
    const u32 capacity  = preallocate ? elementSize * preallocate : elementSize * 1;
    vecHeader_t* header = zalloc(sizeof(vecHeader_t) + capacity);

    header->capacity    = capacity;
    header->count       = 0;
    header->elementSize = elementSize;

    return header + 1;
}

void *_vecFromArray(const void *data, const u32 count, const u32 elementSize) {
    const u32 capacity = count * elementSize;
    vecHeader_t* header = zalloc(sizeof(vecHeader_t) + capacity);

    header->capacity    = capacity;
    header->count       = count;
    header->elementSize = elementSize;

    memcpy(header + 1, data, capacity);
    return header + 1;
}

void *_vecValidateCapacity(const void *vec, const u32 count, const u32 elementSize) {
    vecHeader_t *newVecHeader = vecHeader(vec);
    const u32 wantedCapacity = (newVecHeader->count + count) * elementSize;

    if (newVecHeader->capacity >= wantedCapacity) {
        newVecHeader = newVecHeader + 1;
        return newVecHeader;
    }

    u32 newCapacity = newVecHeader->capacity ? newVecHeader->capacity * 2 : wantedCapacity;
    while (newCapacity < wantedCapacity)
        newCapacity *= 2;

    vecHeader_t* newHeader = zalloc(sizeof(vecHeader_t) + newCapacity);

    if (!newHeader)
        return NULL;

    const u32 oldSize = sizeof(vecHeader_t) + newVecHeader->count * elementSize;
    memcpy(newHeader, newVecHeader, oldSize);

    free(newVecHeader);

    newHeader->capacity = newCapacity;
    newVecHeader = newHeader + 1;
    return newVecHeader;
}
