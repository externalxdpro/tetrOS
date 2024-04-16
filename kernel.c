#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

enum vga_color {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_WHITE,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

static const size_t VGA_WIDTH  = 80;
static const size_t VGA_HEIGHT = 25;

size_t    tty_row;
size_t    tty_column;
uint8_t   tty_color;
uint16_t *tty_buffer;

void tty_initialize() {
    tty_row    = 0;
    tty_column = 0;
    tty_color  = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    tty_buffer = (uint16_t *)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t i = y * VGA_WIDTH + x;
            tty_buffer[i]  = vga_entry(' ', tty_color);
        }
    }
}

void tty_setcolor(uint8_t color) { tty_color = color; }

void tty_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t i = y * VGA_WIDTH + x;
    tty_buffer[i]  = vga_entry(c, color);
}

void tty_clearline(size_t y) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        tty_putentryat(' ', tty_color, x, y);
    }
}

void tty_scroll() {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t curr = y * VGA_WIDTH + x;
            tty_putentryat(tty_buffer[curr], tty_color, x, y - 1);
        }
    }
    tty_clearline(VGA_HEIGHT - 1);
}

void tty_putchar(char c) {
    if (c == '\n') {
        tty_row++;
        tty_column = 0;
    } else {
        tty_putentryat(c, tty_color, tty_column, tty_row);
    }

    if (++tty_column == VGA_WIDTH) {
        tty_column = 0;
        if (++tty_row == VGA_HEIGHT) {
            tty_scroll();
            tty_row = VGA_HEIGHT - 1;
        }
    } else if (tty_row == VGA_HEIGHT) {
        tty_scroll();
        tty_row = VGA_HEIGHT - 1;
    }
}

void tty_write(const char *data, size_t n) {
    for (size_t i = 0; i < n; i++) {
        tty_putchar(data[i]);
    }
}

void tty_writestring(const char *string) { tty_write(string, strlen(string)); }

void kernel_main() {
    tty_initialize();

    tty_writestring("Hello, world!\n");
    tty_writestring("sssssssssssssssssssssssssssssss\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("Hello, world!\n");
    tty_writestring("bub");
}
