#ifndef IDT_H_
#define IDT_H_

// Includes
#include <stdint.h>

// A struct representing a IDT entry
struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

// A struct representing a IDT pointer
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Functions
void idt_load();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();

#endif // IDT_H_
