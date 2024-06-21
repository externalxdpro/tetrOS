// Includes
#include "idt.h"
#include "string.h"

// Initialize an IDT entry array and an IDT pointer
struct idt_entry idt[256];
struct idt_ptr   idtp;

// Get function from boot.s
extern void idt_load();

// Set the gate of a specific IDT entry with the specified arguments
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = ((base >> 16) & 0xFFFF);
    idt[num].sel     = sel;
    idt[num].always0 = 0;
    idt[num].flags   = flags;
}

// Sets up IDT
void idt_install() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = &idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    idt_load();
}
