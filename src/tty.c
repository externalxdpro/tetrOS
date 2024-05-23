#include <stdbool.h>
#include <stdint.h>

#include "string.h"
#include "tty.h"
#include "vga.h"

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

void tty_clearscreen() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        tty_clearline(y);
    }
    tty_row    = 0;
    tty_column = 0;
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
