//
// Created by auggiethecat on 5/2/26.
//

#ifndef _VECTOR_H
#define _VECTOR_H

#include <bdk.h>

typedef struct _vecHeader_t {
    u32 capacity;
    u32 count;
    u32 elementSize;
} vecHeader_t;

typedef void* vector_t;

#define          newVec(type, preallocate) _vecInit(sizeof(type), preallocate)
#define newVecFromArray(type, data, count) _vecFromArray(data, count, sizeof(type))

#define vecHeader(vec) ((vecHeader_t*)(vec) - 1)

#define       vecGetCount(vec) (vecHeader(vec)->count)
#define vecGetElementSize(vec) (vecHeader(vec)->elementSize)
#define    vecGetCapacity(vec) (vecHeader(vec)->capacity / vecHeader(vec)->elementSize)

#define   vecAppend(vec, element) ( \
    (vec) = _vecValidateCapacity(vec, 1, sizeof(element)), \
    (vec)[vecHeader(vec)->count] = (element), \
    &(vec)[vecHeader(vec)->count++])
#define      vecAdd(vec, element) (vecAppend(vec, element))
#define     vecPush(vec, element) (vecAppend(vec, element))
#define vecPushBack(vec, element) (vecAppend(vec, element))

#define     vecPop(vec) (vecHeader(vec)->count -= 1)
#define vecPopBack(vec) (vecPop(vec))

#define vecForEach(type, varName, vecPtr) for (type varName = vecPtr; ((u8*) (varName) - (u8*) (vecPtr)) < (vecHeader(vecPtr)->count); (varName++))

#define  vecFree(vec) free(vec); (vec) = NULL;
#define vecClear(vec) (vecHeader(vec)->count = 0)

void *_vecInit(u32 typeSize, u32 preallocate);
void *_vecFromArray(void *data, u32 count, u32 elementSize);
void *_vecValidateCapacity(void *vec, u32 count, u32 elementSize);

#endif //_VECTOR_H