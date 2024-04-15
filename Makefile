all:
	binutils/bin/i686-elf-as boot.s -o boot.o
	binutils/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	binutils/bin/i686-elf-gcc -T linker.ld -o tetris-os.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	mkdir -p isodir/boot/grub
	cp tetris-os.bin isodir/boot/tetris-os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o tetris-os.iso isodir

run:
	qemu-system-i386 -cdrom tetris-os.iso
