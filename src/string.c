// Includes
#include "string.h"
#include "system.h"

// Converts int to string
char *itoa(int32_t x, char *s, size_t sz) {
    if (sz < 20) {
        panic("ITOA BUFFER TOO SMALL");
    }

    uint32_t tmp;
    int32_t  i, j;

    tmp = x;
    i   = 0;

    do {
        tmp    = x % 10;
        s[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (x /= 10);
    s[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp  = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return s;
}

// Sets a specific amount of memory with a value
void memset(void *dst, uint8_t value, size_t n) {
    uint8_t *d = dst;

    while (n--) {
        *d++ = value;
    }
}

// Copy memory from a source to a destination
void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t       *d = dst;
    const uint8_t *s = src;

    while (n-- > 0) {
        *d++ = *s++;
    }

    return d;
}

// Move memory from a source to a destination
void *memmove(void *dst, const void *src, size_t n) {
    if (dst < src) {
        return memcpy(dst, src, n);
    }

    uint8_t       *d = dst;
    const uint8_t *s = src;

    for (size_t i = n; i > 0; i--) {
        d[i - 1] = s[i - 1];
    }

    return dst;
}

// Get the length of a string
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}
