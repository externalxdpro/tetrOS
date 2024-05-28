#include "string.h"

char *itoa(uint32_t num, char *str, int base) {
    int  i          = 0;
    bool isNegative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i]   = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        isNegative = true;
        num        = -num;
    }

    while (num != 0) {
        int rem  = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num      = num / base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    int start = 0;
    int end   = i - 1;
    while (start < end) {
        char temp  = str[start];
        str[start] = str[end];
        str[end]   = temp;
        end--;
        start++;
    }

    return str;
}

void memset(void *dst, uint8_t value, size_t n) {
    uint8_t *d = dst;

    while (n--) {
        *d++ = value;
    }
}

void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t       *d = dst;
    const uint8_t *s = src;

    while (n-- > 0) {
        *d++ = *s++;
    }

    return d;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}
