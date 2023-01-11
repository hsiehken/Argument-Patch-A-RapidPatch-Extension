#include "FixedPatchTrigger.h"

#include "FirmwarePatches.h"

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

typedef struct PatchItem {
    bool isPatched;
    RAPID_PATCH_RETVAL (*patch)(size_t stackFrame);
} PatchItem;


PatchItem patchList[FIXED_POINT_TRIGGERS];

void fixedPointPatchInstaller() {
    // We simulate reading and installing of a RapidPatch configuration over here

    patchList[0].isPatched = true;
    patchList[0].patch = add4_argument_patch;
}

void fixedPointTrigger(size_t functionID) {
    if(functionID >= FIXED_POINT_TRIGGERS) {
        // illegal access!
        // in practical use-cases, we can add some kernel log here
        return;
    }

    if(patchList[functionID].isPatched) {
        // Here we dispatch the patch as a dispatcher

        // initialize patching runtime, in real RapidPatch implementation, they pushed the patched function's context
        // try to get stack frame of the patched function
        uint64_t rbp64;
        asm ("movq %%rbp, %[rbp64]": [rbp64] "=m"(rbp64));

        // uint64_t rip64; // the rip on the current patch dispatcher
        // asm volatile("1: lea 1b(%%rip), %0;": "=a"(rip64));

        uint64_t stackFrameTarget = rbp64
                + sizeof(size_t) /* countervail push ebp (stack-frame building) */
                + sizeof(size_t) /* countervail eip (return address) */;

        // in a real RapidPatch implementation, we should start an eBPF VM as patch runtime over here
        // as POC, we call the patch function directly
        RAPID_PATCH_RETVAL patchResult = patchList[functionID].patch(stackFrameTarget);

        switch(patchResult) {
            case OP_PASS: // return and continue to the patched function
                return;

            case OP_DROP: // skip the patched function (not supported in this POC)
                // e.g., x86: jmp ebp+4
                // why ebp+4? About stack-frame please refer to: https://en.wikibooks.org/wiki/X86_Disassembly/Functions_and_Stack_Frames
                break;
            case OP_REDIRECT: // return to, e.g., an error handler or another function (not supported in this POC)
                break;
            default:
                // invalid return value, perhaps the installed patch is broken
                // in the real implementation, we can consider removing the patch

                // e.g., log kernel log for the broken patch here
                fixedPointPatchRemover(functionID);
                break;
        }
    }
}

void fixedPointPatchRemover(const size_t functionID) {
    // should be atomic operations
    // however, we are running this POC with a single thread, so it's enough
    patchList[functionID].isPatched = false;
}