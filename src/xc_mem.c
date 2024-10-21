#include <string.h>
#include "xc.h"

#ifdef XC_DEBUG_MEM

#ifdef malloc
#undef malloc
#endif // !malloc

#ifdef realloc
#undef realloc
#endif // !realloc

#ifdef free
#undef free
#endif // !free

XCLIB usize xcDebug_BlockSize(XCMemBlock block) {
    return *((usize *)block - 1);
}

XCLIB XCMemBlock xcDebug_malloc(usize size) {
    XCMemBlock ptr = malloc(size + sizeof(usize));
    if (!ptr)
        return ptr;
    *(usize *)ptr = size;
    return (XCMemBlock)((usize *)ptr + 1);
}

XCLIB XCMemBlock xcDebug_calloc(usize count, usize size) {
    XCMemBlock ptr = xcDebug_malloc(count * size);
    if (!ptr)
        return ptr;
    memset(ptr, 0, count * size);
    return ptr;
}

XCLIB XCMemBlock xcDebug_realloc(XCMemBlock block, usize size) {
    XCMemBlock ptr;
    if (block)
        ptr = realloc((usize *)block - 1, size + sizeof(usize));
    else
        ptr = realloc(NULL, size + sizeof(usize));
    if (!ptr)
        return ptr;
    *(usize *)ptr = size;
    return (XCMemBlock)((usize *)ptr + 1);
}

XCLIB void xcDebug_free(XCMemBlock block) {
    free((usize *)block - 1);
}

#endif
