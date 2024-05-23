#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#define HIBIT(_x) (31 - __builtin_clz((_x)))

#define BIT_SET(_v, _n, _x)                                                    \
    __extension__({                                                            \
        __typeof__(_v) __v = (_v);                                             \
        (__v ^ ((-(_x) ^ __v) & (1 << (_n))));                                 \
    })

#ifndef asm
#define asm __asm__ volatile
#endif

static inline uint8_t inportb(uint16_t port) {
    uint8_t r;
    asm("inb %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

static inline void outportb(uint16_t port, uint8_t data) {
    asm("outb %1, %0" : : "dN"(port), "a"(data));
}

#endif // UTIL_H_
