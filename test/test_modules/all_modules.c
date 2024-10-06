#include "test_xcCompare.c"
#include "test_xcDebug_BlockSize.c"
#include "test_xcArrayInit.c"
#include "test_xcArrayInitFromCopy.c"
#include "test_xcArrayInitFromData.c"
#include "test_xcArrayNew.c"
#include "test_xcArrayNewFromCopy.c"
#include "test_xcArrayNewFromData.c"
#include "test_xcArrayDestroy.c"
#include "test_xcArrayFree.c"
#include "test_xcArrayGet.c"
#include "test_xcArrayFind.c"
#include "test_xcArrayFindAll.c"
#include "test_xcArrayFilter.c"
#include "test_xcArrayAppend.c"
#include "test_xcArrayInsert.c"
#include "test_xcArrayExtend.c"
#include "test_xcArraySet.c"
#include "test_xcArraySwap.c"
#include "test_xcArrayMove.c"

void addTests(void) {
    test_xcCompare__addTests();
    test_xcDebug_BlockSize__addTests();
    test_xcArrayInit__addTests();
    test_xcArrayInitFromCopy__addTests();
    test_xcArrayInitFromData__addTests();
    test_xcArrayNew__addTests();
    test_xcArrayNewFromCopy__addTests();
    test_xcArrayNewFromData__addTests();
    test_xcArrayDestroy__addTests();
    test_xcArrayFree__addTests();
    test_xcArrayGet__addTests();
    test_xcArrayFind__addTests();
    test_xcArrayFindAll__addTests();
    test_xcArrayFilter__addTests();
    test_xcArrayAppend__addTests();
    test_xcArrayInsert__addTests();
    test_xcArrayExtend__addTests();
    test_xcArraySet__addTests();
    test_xcArraySwap__addTests();
    test_xcArrayMove__addTests();
}
