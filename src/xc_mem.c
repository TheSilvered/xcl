#include <string.h>
#include "xc.h"

#ifdef XC_DEBUG_MEM

#ifdef malloc
#undef malloc
#endif

#ifdef realloc
#undef realloc
#endif

#ifdef free
#undef free
#endif

usize xcDebug_BlockSize(void *block)
{
    return *((usize *)block - 1);
}

void *xcDebug_malloc(usize size) {
    void *ptr = malloc(size + sizeof(usize));
    if (!ptr)
        return ptr;
    *(usize *)ptr = size;
    return (void *)((usize *)ptr + 1);
}

void *xcDebug_calloc(usize count, usize size) {
    void *ptr = xcDebug_malloc(count * size);
    if (!ptr)
        return ptr;
    memset(ptr, 0, count * size);
    return ptr;
}

void *xcDebug_realloc(void *block, usize size) {
    void *ptr;
    if (block)
        ptr = realloc((usize *)block - 1, size + sizeof(usize));
    else
        ptr = realloc(NULL, size + sizeof(usize));
    if (!ptr)
        return ptr;
    *(usize *)ptr = size;
    return (void *)((usize *)ptr + 1);
}

void xcDebug_free(void *block) {
    free((usize *)block - 1);
}

#define malloc(size) xcDebug_malloc(size)
#define realloc(block, size) xcDebug_realloc(block, size)
#define free(block) xcDebug_free(block)

#endif