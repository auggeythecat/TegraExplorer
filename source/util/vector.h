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

#ifndef _VECTOR_H
#define _VECTOR_H

#include <utils/types.h>

typedef struct _vecHeader_t {
    u32 capacity;
    u32 count;
    u32 elementSize;
} vecHeader_t;

typedef void* vector_t;

#define          newVec(type, preallocate) _vecInit(sizeof(type), preallocate)
#define newVecFromArray(type, data, count) _vecFromArray(data, count, sizeof(type))

#define vecHeader(vec) ((vecHeader_t*)(vec) - 1)

#define       vecGetCount(vec) (vecHeader(vec)->count                                 )
#define vecGetElementSize(vec) (                           vecHeader(vec)->elementSize)
#define    vecGetCapacity(vec) (vecHeader(vec)->capacity / vecHeader(vec)->elementSize)

    // TODO: This will segfault if no memory I think?
    // I wonder how I should fix that...
#define   vecAppend(vec, element) ( \
    (vec) = _vecValidateCapacity(vec, 1, sizeof(element)), \
    (vec)[vecHeader(vec)->count] = (element), \
    (vec)[vecHeader(vec)->count++])
#define      vecAdd(vec, element) (vecAppend(vec, element))
#define     vecPush(vec, element) (vecAppend(vec, element))
#define vecPushBack(vec, element) (vecAppend(vec, element))

#define     vecPop(vec) (vecHeader(vec)->count -= 1)
#define vecPopBack(vec) (vecPop(vec))

#define vecForEach(type, varName, vecPtr) for (type varName = vecPtr; \
    ((u8*) (varName) - (u8*) (vecPtr)) < (vecHeader(vecPtr)->count); (varName++))

#define  vecFree(vec) free(vec); (vec) = NULL;
#define vecClear(vec) (vecHeader(vec)->count = 0)

void *_vecInit(u32 typeSize, u32 preallocate);
void *_vecFromArray(const void *data, u32 count, u32 elementSize);
void *_vecValidateCapacity(const void *vec, u32 count, u32 elementSize);

#endif //_VECTOR_H
