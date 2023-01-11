#include <stdio.h>

#include "FixedPatchTrigger.h"

#define FUNC_ID_ADD4 0

int add4(int a, int b, int c, int d) {
    fixedPointTrigger(FUNC_ID_ADD4);

    return a + b + c + d;
}

int main() {
    int result = add4(23, 24, 26, 27);
    printf("(Before Patch) add4(23, 24, 26, 27) Get result=%d.\n", result);

    printf("Installing fixed point patches...\n");
    fixedPointPatchInstaller();
    printf("Installed fixed point patches.\n");

    printf("add4 function have been patched by a fixed point patch\n");
    printf("Press 'Enter' key to see the result...\n");
    getchar();

    result = add4(23, 24, 26, 27);
    printf("(After Patch) Get result=%d. Should be=%d\n", result, 23+24+26+27);

    return 0;
}
