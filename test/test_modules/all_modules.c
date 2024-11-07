// Memory allocation

#include "test_xcDebug_BlockSize.c"

// Utility functions

#include "test_xcCompare.c"
#include "test_xcMin.c"
#include "test_xcMax.c"
// #include "test_xcHash.c"

// Dynamic arrays

#include "test_xcArrayInit.c"
#include "test_xcArrayInitFromCopy.c"
#include "test_xcArrayInitFromData.c"
#include "test_xcArrayNew.c"
#include "test_xcArrayNewFromCopy.c"
#include "test_xcArrayNewFromData.c"
// #include "test_xcArrayViewInit.c"
// #include "test_xcArrayViewInitFromData.c"
// #include "test_xcArrayViewNew.c"
// #include "test_xcArrayViewNewFromData.c"
// #include "test_xcArrayViewMake.c"
// #include "test_xcArrayViewMakeFromData.c"
#include "test_xcArrayDestroy.c"
#include "test_xcArrayFree.c"
// #include "test_xcArrayViewFree.c"
#include "test_xcArrayGet.c"
#include "test_xcArrayFind.c"
#include "test_xcArrayFindAll.c"
#include "test_xcArrayFilter.c"
// #include "test_xcArrayViewGet.c"
// #include "test_xcArrayViewFind.c"
// #include "test_xcArrayViewFindAll.c"
// #include "test_xcArrayViewFilter.c"
#include "test_xcArrayAppend.c"
#include "test_xcArrayInsert.c"
#include "test_xcArrayExtend.c"
#include "test_xcArraySet.c"
#include "test_xcArraySwap.c"
#include "test_xcArrayMove.c"
#include "test_xcArraySort.c"
// #include "test_xcArrayDel.c"
// #include "test_xcArrayClear.c"
// #include "test_xcArrayRemove.c"
// #include "test_xcArrayRemoveAll.c"
// #include "test_xcArrayRemoveFilter.c"
// #include "test_xcArrayNext.c"
// #include "test_xcArrayViewNext.c"
// #include "test_xcArrayIdxCheck.c"
// #include "test_xcArrayRefToIdx.c"
// #include "test_xcArrayBisect.c"
// #include "test_xcArrayBisectRight.c"
// #include "test_xcArrayBisectEx.c"
// #include "test_xcArrayBisectRightEx.c"
// #include "test_xcArrayViewIdxCheck.c"
// #include "test_xcArrayViewRefToIdx.c"
// #include "test_xcArrayViewBisect.c"
// #include "test_xcArrayViewBisectRight.c"
// #include "test_xcArrayViewBisectEx.c"
// #include "test_xcArrayViewBisectRightEx.c"

// Boolean array

// #include "test_xcBoolArraySize.c"
// #include "test_xcBoolArrayNew.c"
// #include "test_xcBoolArrayFree.c"
// #include "test_xcBoolArrayGet.c"
// #include "test_xcBoolArraySet.c"
// #include "test_xcBoolArraySetAll.c"

// Hash map

// #include "test_xcMapInit.c"
// #include "test_xcMapNew.c"
// #include "test_xcMapDestroy.c"
// #include "test_xcMapFree.c"
// #include "test_xcMapGet.c"
// #include "test_xcMapAdd.c"
// #include "test_xcMapSet.c"
// #include "test_xcMapDel.c"
// #include "test_xcMapClear.c"
// #include "test_xcMapNext.c"
// #include "test_xcMapNextPair.c"

// Strings

// #include "test_xcStrInit.c"
// #include "test_xcStrInitEmpty.c"
// #include "test_xcStrInitFromData.c"
// #include "test_xcStrNew.c"
// #include "test_xcStrNewEmpty.c"
// #include "test_xcStrNewFromData.c"
// #include "test_xcStrViewInitFromXCStr.c"
// #include "test_xcStrDestroy.c"
// #include "test_xcStrFree.c"

void addTests(void) {
    // Memory allocation

    test_xcDebug_BlockSize__addTests();

    // Utility functions

    test_xcCompare__addTests();
    test_xcMin__addTests();
    test_xcMax__addTests();
    // #include "test_xcHash.c"

    // Dynamic arrays

    test_xcArrayInit__addTests();
    test_xcArrayInitFromCopy__addTests();
    test_xcArrayInitFromData__addTests();
    test_xcArrayNew__addTests();
    test_xcArrayNewFromCopy__addTests();
    test_xcArrayNewFromData__addTests();
    // test_xcArrayViewInit__addTests();
    // test_xcArrayViewInitFromData__addTests();
    // test_xcArrayViewNew__addTests();
    // test_xcArrayViewNewFromData__addTests();
    // test_xcArrayViewMake__addTests();
    // test_xcArrayViewMakeFromData__addTests();
    test_xcArrayDestroy__addTests();
    test_xcArrayFree__addTests();
    // #include "test_xcArrayViewFree.c"
    test_xcArrayGet__addTests();
    test_xcArrayFind__addTests();
    test_xcArrayFindAll__addTests();
    test_xcArrayFilter__addTests();
    // test_xcArrayViewGet__addTests();
    // test_xcArrayViewFind__addTests();
    // test_xcArrayViewFindAll__addTests();
    // test_xcArrayViewFilter__addTests();
    test_xcArrayAppend__addTests();
    test_xcArrayInsert__addTests();
    test_xcArrayExtend__addTests();
    test_xcArraySet__addTests();
    test_xcArraySwap__addTests();
    test_xcArrayMove__addTests();
    test_xcArraySort__addTests();
    // test_xcArrayDel__addTests();
    // test_xcArrayClear__addTests();
    // test_xcArrayRemove__addTests();
    // test_xcArrayRemoveAll__addTests();
    // test_xcArrayRemoveFilter__addTests();
    // test_xcArrayNext__addTests();
    // test_xcArrayViewNext__addTests();
    // test_xcArrayIdxCheck__addTests();
    // test_xcArrayRefToIdx__addTests();
    // test_xcArrayBisect__addTests();
    // test_xcArrayBisectRight__addTests();
    // test_xcArrayBisectEx__addTests();
    // test_xcArrayBisectRightEx__addTests();
    // test_xcArrayViewIdxCheck__addTests();
    // test_xcArrayViewRefToIdx__addTests();
    // test_xcArrayViewBisect__addTests();
    // test_xcArrayViewBisectRight__addTests();
    // test_xcArrayViewBisectEx__addTests();
    // test_xcArrayViewBisectRightEx__addTests();

    // Boolean array

    // test_xcBoolArraySize__addTests();
    // test_xcBoolArrayNew__addTests();
    // test_xcBoolArrayFree__addTests();
    // test_xcBoolArrayGet__addTests();
    // test_xcBoolArraySet__addTests();
    // test_xcBoolArraySetAll__addTests();

    // Hash map

    // test_xcMapInit__addTests();
    // test_xcMapNew__addTests();
    // test_xcMapDestroy__addTests();
    // test_xcMapFree__addTests();
    // test_xcMapGet__addTests();
    // test_xcMapAdd__addTests();
    // test_xcMapSet__addTests();
    // test_xcMapDel__addTests();
    // test_xcMapClear__addTests();
    // test_xcMapNext__addTests();
    // test_xcMapNextPair__addTests();

    // Strings

    // test_xcStrInit__addTests();
    // test_xcStrInitEmpty__addTests();
    // test_xcStrInitFromData__addTests();
    // test_xcStrNew__addTests();
    // test_xcStrNewEmpty__addTests();
    // test_xcStrNewFromData__addTests();
    // test_xcStrViewInitFromXCStr__addTests();
    // test_xcStrDestroy__addTests();
    // test_xcStrFree__addTests();
}
