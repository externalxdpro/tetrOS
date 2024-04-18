#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "idt.h"
#include "tty.h"

#if defined(__linux__)
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main() {
    idt_install();

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
