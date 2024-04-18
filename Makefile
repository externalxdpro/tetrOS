CC=./binutils/bin/i686-elf-gcc
ASM=./binutils/bin/i686-elf-as
LD=./binutils/bin/i686-elf-ld

GFLAGS=
CCFLAGS=-std=c11 -O2 -g
CCFLAGS+=-Wall -Wextra -Wpedantic -Wstrict-aliasing -Wno-pointer-arith -Wno-unused-parameter
CCFLAGS+=-ffreestanding
ASFLAGS=
LDFLAGS=

KERNEL_C_SRCS=$(wildcard src/*.c)
KERNEL_S_SRCS=$(wildcard src/*.s)
KERNEL_OBJS=$(KERNEL_C_SRCS:.c=.o) $(KERNEL_S_SRCS:.s=.o)

KERNEL=kernel.bin
ISO=boot.iso

all: dirs kernel

dirs:
	mkdir -p bin
	mkdir -p isodir/boot/grub

clean:
	rm -f ./**/*.o
	rm -f ./**/*.bin
	rm -f ./*.iso
	rm -rf ./isodir/

%.o: %.c
	$(CC) -o $@ -c $< $(GFLAGS) $(CCFLAGS)

%.o: %.s
	$(ASM) -o $@ -c $< $(GFLAGS) $(ASFLAGS)

kernel: $(KERNEL_OBJS)
	$(LD) -o ./bin/$(KERNEL) $^ $(LDFLAGS) -Tsrc/linker.ld

iso: kernel
	cp bin/$(KERNEL) isodir/boot/
	cp src/grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(ISO) isodir
