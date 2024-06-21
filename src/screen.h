#ifndef SCREEN_H_
#define SCREEN_H_

// Includes
#include "string.h"
#include "util.h"

// Defines
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define COLOUR(_r, _g, _b)                                                     \
    ((uint8_t)((((_r) & 0x7) << 5) | (((_g) & 0x7) << 2) | (((_b) & 0x3) << 0)))

#define COLOUR_R(_index) (((_index) >> 5) & 0x7)
#define COLOUR_G(_index) (((_index) >> 2) & 0x7)
#define COLOUR_B(_index) (((_index) >> 0) & 0x3)

#define COLOUR_ADD(_index, _d)                                                 \
    __extension__({                                                            \
        __typeof__(_index) _c  = (_index);                                     \
        __typeof__(_d)     __d = (_d);                                         \
        COLOUR(CLAMP(COLOUR_R(_c) + __d, 0, 7),                                \
               CLAMP(COLOUR_G(_c) + __d, 0, 7),                                \
               CLAMP(COLOUR_B(_c) + __d, 0, 3));                               \
    })

extern uint8_t _screen_buffer[SCREEN_SIZE];

#define screen_buffer() (_screen_buffer)

#define screen_set(_p, _x, _y)                                                 \
    (_screen_buffer[((_y) * SCREEN_WIDTH + (_x))] = (_p))

#define screen_offset(_x, _y) (screen_buffer()[(_y) * SCREEN_WIDTH + (_x)])

#define screen_fill(_c, _x, _y, _w, _h)                                        \
    do {                                                                       \
        __typeof__(_x) __x    = (_x);                                          \
        __typeof__(_y) __y    = (_y);                                          \
        __typeof__(_w) __w    = (_w);                                          \
        __typeof__(_y) __ymax = __y + (_h);                                    \
        __typeof__(_c) __c    = (_c);                                          \
        for (; __y < __ymax; __y++) {                                          \
            memset(&screen_buffer()[__y * SCREEN_WIDTH + __x], __c, __w);      \
        }                                                                      \
    } while (0)

// Functions
void screen_update();
void screen_clear(uint8_t colour);
void screen_install();

#endif // SCREEN_H_
