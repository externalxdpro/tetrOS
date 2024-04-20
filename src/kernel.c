#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "tty.h"

#if defined(__linux__)
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main() {
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    __asm__ __volatile__("sti");

    tty_initialize();

    tty_writestring("Hello, world!\n");
    tty_writestring("sssssssssssssssssssssssssssssss\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("bub");
}
