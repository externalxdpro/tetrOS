#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#define __MIN_IMPL(_x, _y, _xn, _yn)                                           \
    __extension__({                                                            \
        __typeof__(_x) _xn = (_x);                                             \
        __typeof__(_y) _yn = (_y);                                             \
        (_xn < _yn ? _xn : _yn);                                               \
    })
#define MIN(_x, _y)                                                            \
    __MIN_IMPL(_x, _y, CONCAT(__x, __COUNTER__), CONCAT(__y, __COUNTER__))

#define __MAX_IMPL(_x, _y, _xn, _yn)                                           \
    __extension__({                                                            \
        __typeof__(_x) _xn = (_x);                                             \
        __typeof__(_y) _yn = (_y);                                             \
        (_xn > _yn ? _xn : _yn);                                               \
    })
#define MAX(_x, _y)                                                            \
    __MAX_IMPL(_x, _y, CONCAT(__x, __COUNTER__), CONCAT(__y, __COUNTER__))

#define CLAMP(_x, _mi, _ma) (MAX(_mi, MIN(_x, _ma)))

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
