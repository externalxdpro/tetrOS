#ifndef GDT_H_
#define GDT_H_

// Includes
#include <stdint.h>

// A struct representing a GDT entry
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

// A struct representing a GDT pointer
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Functions
void gdt_flush();
void gdt_set_gate(int32_t num, uint64_t base, uint64_t limit, uint8_t access,
                  uint8_t granularity);
void gdt_install();

#endif // GDT_H_
