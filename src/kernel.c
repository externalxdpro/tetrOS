#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include "util.h"

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
    screen_install();
    timer_install();
    keyboard_install();

    asm("sti");

    while (true) {}
}
