// Includes
#include "screen.h"
#include "string.h"

// The current buffer
static uint8_t *BUFFER = (uint8_t *)0xA0000;

// Temp buffer
uint8_t _screen_buffer[SCREEN_SIZE];

// Defines
#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

// Refreshes the screen
void screen_update() { memcpy(BUFFER, &_screen_buffer, SCREEN_SIZE); }

// Clears the screen
void screen_clear(uint8_t colour) {
    memset(&_screen_buffer, colour, SCREEN_SIZE);
}

// Sets up stuff for the screen
void screen_install() {
    outportb(PALETTE_MASK, 0xFF);
    outportb(PALETTE_WRITE, 0);
    for (uint8_t i = 0; i < 255; i++) {
        outportb(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
}
