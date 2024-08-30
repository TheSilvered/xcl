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

usize XCDebug_BlockSize(void *block)
{
    return *((usize *)block - 1);
}

void *XCDebug_malloc(usize size) {
    void *ptr = malloc(size + sizeof(usize));
    if (!ptr)
        return ptr;
    *(usize *)ptr = size;
    return (void *)((usize *)ptr + 1);
}

void *XCDebug_calloc(usize count, usize size) {
    void *ptr = XCDebug_malloc(count * size);
    if (!ptr)
        return ptr;
    memset(ptr, 0, count * size);
    return ptr;
}

void *XCDebug_realloc(void *block, usize size) {
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

void XCDebug_free(void *block) {
    free((usize *)block - 1);
}

#define malloc XCDebug_malloc
#define realloc XCDebug_realloc
#define free XCDebug_free

#endif