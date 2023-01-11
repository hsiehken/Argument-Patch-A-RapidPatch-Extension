#include <linux/kernel.h>
#include <linux/types.h>

uint64_t dr7Backup;

uint64_t printDr0Dr7(void) {
    uint64_t dr0;
    uint64_t dr7;
    asm volatile ("movq %%dr0, %%rax; movq %%rax, %[dr0]": [dr0] "=m"(dr0)::"%rax");
    asm volatile ("movq %%dr7, %%rax; movq %%rax, %[dr7]": [dr7] "=m"(dr7)::"%rax");

    printk(KERN_INFO "[RapidPatch] DEBUG dr0=0x%zx dr7=0x%zx\n", dr0, dr7);

    return dr7;
}

unsigned int initializeHardwareDebugger(size_t breakPoint) {
    dr7Backup = printDr0Dr7();
    uint64_t updatedDr7 = dr7Backup;
    updatedDr7 |= 0b10; // Set GD bit

    printk(KERN_INFO "[RapidPatch] Setting up debugger to address: 0x%zx (dr7=0x%zx)\n", breakPoint, updatedDr7);

    // do write debug register
    asm ("movq %0, %%rax; movq %%rax, %%dr0"::"r"(breakPoint):"%rax");
    asm ("xor %%rax,%%rax;\n\t"
         "movq %0, %%rax;\n\t"
         "movq %%rax, %%dr7;" :: "r" (updatedDr7):);

    printDr0Dr7();

    return 0;
}

unsigned int restoreHardwareDebugger(void) {
    // restore dr7
    asm ("movq %0, %%rax; movq %%rax, %%dr7"::"r"(dr7Backup):"%rax");

    printDr0Dr7();

    return 0;
}