#include "LePatch.h"
//
// The Glorious Patches in this file...
//

void add4_argument_patch(ExecutionContext *handle) {
    // The improvement of argument patch over here is that, the patch get more guaranteed execution context
    // which is useful for some cases. E.g.,:
    // We can check where which function is calling us,
    // The context of the caller function (starts from handle->eflags+8)

    // Since it is a Poc, suddenly, we somehow would like to patch the function with b+=100
    handle->rsi += 100; // b is the second argument, is thus put in rsi
}
