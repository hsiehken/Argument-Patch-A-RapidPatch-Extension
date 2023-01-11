#include "DynamicPatchTrigger.h"

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/traps.h>
#include <asm/desc_defs.h>
#include <asm/desc.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>

#include "LePatch.h"

//PGFAULT_NR is the interrupt number of page fault. It is platform specific.
#if defined(CONFIG_X86_64)
#define IRQ_NUMBER X86_TRAP_BP
#else
#error This module is only for X86_64 kernel
#endif

#define IRQ_NUMBER X86_TRAP_DB // We are installing out debug monitor to x86's debug exception

static unsigned long new_idt_table_page;
static struct desc_ptr default_idtr;

extern void patch_trigger(void);

static void doLoadIdt(void *info) {
    struct desc_ptr *idtr_ptr = (struct desc_ptr *) info;
    load_idt(idtr_ptr);
}

extern void patch_trigger(void);

size_t patch_dispatcher(ExecutionContext *registers) {
    // In the real implementation, we should search for patch in this function
    // here we simulate that we found an Argument Patch for the "add4" function

    // And, we also have to found out, from which debug register was the breakpoint being set
    // here, as it is a PoC, we knew it must come from dr0, we disable it directly with mask xFFFFFFFFFFFFFFFC,
    // detail please refer to https://en.wikipedia.org/wiki/X86_debug_register

    // Disable hardware breakpoint temporarily otherwise, iretq will cause infinity loops
    asm volatile("mov %%dr7, %%rax; and $0xFFFFFFFFFFFFFFFC, %%rax; mov %%rax, %%dr7":: :"rax");

    return (size_t)add4_argument_patch;
}

int registerDebugHandler(void) {
    printk(KERN_INFO "[RapidPatch] Hooking IRQ %d\n", IRQ_NUMBER);

    struct desc_ptr idtr;
    gate_desc *old_idt, *new_idt;

    //save the oringinal idtr
    store_idt(&default_idtr);

    //read the content of idtr register and get the address of old IDT table
    old_idt = (gate_desc *) default_idtr.address; //'default_idtr' is initialized in 'my_virt_drv_init'

    //allocate a page to store the new IDT table
    new_idt_table_page = __get_free_page(GFP_KERNEL);
    if (!new_idt_table_page)
        return -ENOMEM;

    idtr.address = new_idt_table_page;
    idtr.size = default_idtr.size;

    // duplicate the old IDT table
    new_idt = (gate_desc *) idtr.address;
    memcpy(new_idt, old_idt, idtr.size);
    gate_desc *old = (gate_desc *) default_idtr.address;
    pack_gate(&new_idt[IRQ_NUMBER], GATE_INTERRUPT, (size_t) myDebugMonitor, old[IRQ_NUMBER].bits.dpl,
              old[IRQ_NUMBER].bits.ist, old[IRQ_NUMBER].segment);

    // load IDT for all the processors
    load_idt(&idtr);
    smp_call_function(doLoadIdt, (void *) &idtr, 1); // wait till all are finished

    return 0;
}

void deregisterDebugHandler(void) {
    struct desc_ptr idtRegister;
    store_idt(&idtRegister);
    //if the current idt is not the default one, restore the default one
    if (idtRegister.address != default_idtr.address || idtRegister.size != default_idtr.size) {
        load_idt(&default_idtr);
        smp_call_function(doLoadIdt, (void *) &default_idtr, 1);
        free_page(new_idt_table_page);
    }
}

MODULE_LICENSE("Dual BSD/GPL");