#include "irq.h"
#include "idt.h"
#include "isr.h"
#include "util.h"

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

static void (*irq_routines[16])() = {0};

void irq_install_handler(int irq, void (*handler)(struct registers *r)) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) { irq_routines[irq] = 0; }

void irq_remap() {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

void irq_install() {
    irq_remap();

    idt_set_gate(32, (uintptr_t)_irq0, 0x08, 0x8E);
    idt_set_gate(33, (uintptr_t)_irq1, 0x08, 0x8E);
    idt_set_gate(34, (uintptr_t)_irq2, 0x08, 0x8E);
    idt_set_gate(35, (uintptr_t)_irq3, 0x08, 0x8E);
    idt_set_gate(36, (uintptr_t)_irq4, 0x08, 0x8E);
    idt_set_gate(37, (uintptr_t)_irq5, 0x08, 0x8E);
    idt_set_gate(38, (uintptr_t)_irq6, 0x08, 0x8E);
    idt_set_gate(39, (uintptr_t)_irq7, 0x08, 0x8E);
    idt_set_gate(40, (uintptr_t)_irq8, 0x08, 0x8E);
    idt_set_gate(41, (uintptr_t)_irq9, 0x08, 0x8E);
    idt_set_gate(42, (uintptr_t)_irq10, 0x08, 0x8E);
    idt_set_gate(43, (uintptr_t)_irq11, 0x08, 0x8E);
    idt_set_gate(44, (uintptr_t)_irq12, 0x08, 0x8E);
    idt_set_gate(45, (uintptr_t)_irq13, 0x08, 0x8E);
    idt_set_gate(46, (uintptr_t)_irq14, 0x08, 0x8E);
    idt_set_gate(47, (uintptr_t)_irq15, 0x08, 0x8E);
}

void irq_handler(struct registers *r) {
    void (*handler)(struct registers *r);

    handler = irq_routines[r->int_no - 32];
    if (handler) {
        handler(r);
    }

    if (r->int_no >= 40) {
        outportb(0xA0, 0x20);
    }
    outportb(0x20, 0x20);
}
