/*
 * Standard in kernel modules
 */
#include <linux/kernel.h>    /* We're doing kernel work */
#include <linux/module.h>    /* Specifically, a module */

#include "DynamicPatchInstaller.h"
#include "DynamicPatchTrigger.h"

int add4(int a, int b, int c, int d) {
    return a + b + c + d;
}


// somehow compiler is performing some magic optimizations on function calls
// (probably inlining since our demo function is too small)
// to make sure the function call is called on expected, let's get our hands dirty: call with assembler
int callAdd4(int a, int b, int c, int d) {
    int result;
    // rdi, rsi, rdx, rcx, r8d, r9d
    asm ("movl %[a], %%edi; \n\t"
         "movl %[b], %%esi; \n\t"
         "movl %[c], %%edx; \n\t"
         "movl %[d], %%ecx; \n\t"
         "callq add4; \n\t"
         "movl %%eax, %[result]; \n\t"
            :[result] "=m"(result)
            :[a]"r"(a), [b]"r"(b), [c]"r"(c), [d]"r"(d)
            :"%rdi", "%rsi", "%rdx", "%rcx", "%rax");

    return result;
}

/*
 * Initialize the module - register the IRQ handler
 */
int init_module() {
    int result = 0;
    printk(KERN_INFO
    "[RapidPatch] Program starting...\n");
    printk(KERN_INFO
    "[RapidPatch] Before Patch: add4(1, 2, 3, 4) = %d\n", callAdd4(23, 24, 26, 27));
    result = registerDebugHandler();
    if (result != 0) {
        return result;
    }

    initializeHardwareDebugger((size_t) add4);

    // We changed the order since it is a kernel module and compiler is optimizing very hard!
    printk(KERN_INFO "[RapidPatch] After Patch: add4(1, 2, 3, 4) = %d\n", callAdd4(27, 26, 24, 23));

    return result;
}

/*
 * Cleanup
 */
void cleanup_module() {
    restoreHardwareDebugger();
    deregisterDebugHandler();
    printk(KERN_INFO
    "[RapidPatch] Program stopped\n");
}

MODULE_LICENSE("GPL");