#include "isr.h"
#include "idt.h"
#include "system.h"
#include "tty.h"

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

void isr_install() {
    idt_set_gate(0, (uintptr_t)_isr0, 0x08, 0x8E);
    idt_set_gate(1, (uintptr_t)_isr1, 0x08, 0x8E);
    idt_set_gate(2, (uintptr_t)_isr2, 0x08, 0x8E);
    idt_set_gate(3, (uintptr_t)_isr3, 0x08, 0x8E);
    idt_set_gate(4, (uintptr_t)_isr4, 0x08, 0x8E);
    idt_set_gate(5, (uintptr_t)_isr5, 0x08, 0x8E);
    idt_set_gate(6, (uintptr_t)_isr6, 0x08, 0x8E);
    idt_set_gate(7, (uintptr_t)_isr7, 0x08, 0x8E);
    idt_set_gate(8, (uintptr_t)_isr8, 0x08, 0x8E);
    idt_set_gate(9, (uintptr_t)_isr9, 0x08, 0x8E);
    idt_set_gate(10, (uintptr_t)_isr10, 0x08, 0x8E);
    idt_set_gate(11, (uintptr_t)_isr11, 0x08, 0x8E);
    idt_set_gate(12, (uintptr_t)_isr12, 0x08, 0x8E);
    idt_set_gate(13, (uintptr_t)_isr13, 0x08, 0x8E);
    idt_set_gate(14, (uintptr_t)_isr14, 0x08, 0x8E);
    idt_set_gate(15, (uintptr_t)_isr15, 0x08, 0x8E);
    idt_set_gate(16, (uintptr_t)_isr16, 0x08, 0x8E);
    idt_set_gate(17, (uintptr_t)_isr17, 0x08, 0x8E);
    idt_set_gate(18, (uintptr_t)_isr18, 0x08, 0x8E);
    idt_set_gate(19, (uintptr_t)_isr19, 0x08, 0x8E);
    idt_set_gate(20, (uintptr_t)_isr20, 0x08, 0x8E);
    idt_set_gate(21, (uintptr_t)_isr21, 0x08, 0x8E);
    idt_set_gate(22, (uintptr_t)_isr22, 0x08, 0x8E);
    idt_set_gate(23, (uintptr_t)_isr23, 0x08, 0x8E);
    idt_set_gate(24, (uintptr_t)_isr24, 0x08, 0x8E);
    idt_set_gate(25, (uintptr_t)_isr25, 0x08, 0x8E);
    idt_set_gate(26, (uintptr_t)_isr26, 0x08, 0x8E);
    idt_set_gate(27, (uintptr_t)_isr27, 0x08, 0x8E);
    idt_set_gate(28, (uintptr_t)_isr28, 0x08, 0x8E);
    idt_set_gate(29, (uintptr_t)_isr29, 0x08, 0x8E);
    idt_set_gate(30, (uintptr_t)_isr30, 0x08, 0x8E);
    idt_set_gate(31, (uintptr_t)_isr31, 0x08, 0x8E);
}

static const char *exceptions[32] = {
    "Divide by zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "OOB",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unrecognized interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
};

void fault_handler(struct registers *r) {
    if (r->int_no < 32) {
        panic(exceptions[r->int_no]);
    }
}
