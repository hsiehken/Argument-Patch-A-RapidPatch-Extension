obj-m += RapidPatch.o
RapidPatch-objs = Main.o DynamicPatchTrigger.o DebugMonitor.o DynamicPatchInstaller.o LePatch.o
KERNEL_HEADERS = /lib/modules/$(shell uname -r)/build

# nams must be installed

all: DebugMonitor.o
	make -C $(KERNEL_HEADERS) M="$(shell pwd)" modules
	kmodsign sha512 ../sign/MOK.priv ../sign/MOK.der RapidPatch.ko
	modinfo RapidPatch.ko

DebugMonitor.o: DebugMonitor.asm
	nasm -f elf64 -o $@ $^ && echo "" > "$(shell pwd)"/.DebugMonitor.o.cmd

clean:
	make -C $(KERNEL_HEADERS)  M="$(shell pwd)" clean
