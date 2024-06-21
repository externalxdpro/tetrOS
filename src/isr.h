#ifndef ISR_H_
#define ISR_H_

// Includes
#include <stdint.h>

// A struct that represents registers
struct registers {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

// Functions
void isr_install();

#endif // ISR_H_
