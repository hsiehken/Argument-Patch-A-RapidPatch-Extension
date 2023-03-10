#include <stdio.h>
#include "FirmwarePatches.h"

RAPID_PATCH_RETVAL add4_argument_patch(ExecutionContext handle) {
    // target function signature: add4(int a, int b, int c, int d)

    // Practically these are only acquired-able via RapidPatch Runtime APIs
    size_t *callerSignature = ((size_t *) (handle - sizeof(size_t))); // i.e. rip register
    size_t *rbp = ((size_t *) (handle - sizeof(size_t) - sizeof(size_t)));
    size_t *callerContext = ((size_t *) (handle - sizeof(size_t) - sizeof(size_t) - sizeof(size_t)));
    // ... more caller context can be found in (callerContext - sizeof(size_t) * n)

    int *d = ((int *) handle);
    int *c = ((int *) (handle + sizeof(int)));
    int *b = ((int *) (handle + sizeof(int) + sizeof(int)));
    int *a = ((int *) (handle + sizeof(int) + sizeof(int) + sizeof(int)));
    printf("Caller's execution context: callerSignature=0x%zx\n", *callerSignature);
    printf("Callee gets arguments from caller: a=%d, b=%d, c=%d, d=%d, \n", *a, *b, *c, *d);

    // our POC patch, when you press 'any key' after installing this patch, result will be = 200
    *b += 100;

    return OP_PASS;
}