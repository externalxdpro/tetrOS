#include "string.h"

void memset(void *dst, uint8_t value, size_t n) {
    uint8_t *d = dst;

    while (n--) {
        *d++ = value;
    }
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}
