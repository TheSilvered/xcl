#include <stdio.h>
#include <string.h>
#include "xc.h"

int main(void) {
    u32 cp = 0x00DF; // ß
    u32 caseExpansion[XC_MAX_CASE_EXPANSION];
    XCUnicodeChInfo chInfo = xcUnicodeGetChInfo(cp);
    XCStr str;
    xcStrInitEmpty(&str);
    usize caseSize = xcUnicodeExpandCase(cp, chInfo.upper, caseExpansion);
    xcStrAppendUnicode(&str, caseExpansion, caseSize);
    printf("%.*s\n", (int)str.size, str.data);

    return 0;
}
