#ifndef STRING_H_
#define STRING_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void   memset(void *, uint8_t, size_t);
void  *memcpy(void *dst, const void *src, size_t n);
size_t strlen(const char *);

#endif // STRING_H_
