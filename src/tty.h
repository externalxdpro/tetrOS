#ifndef TTY_H_
#define TTY_H_

#include <stddef.h>

void tty_initialize(void);
void tty_putchar(char c);
void tty_write(const char *data, size_t n);
void tty_writestring(const char *string);

#endif // TTY_H_
