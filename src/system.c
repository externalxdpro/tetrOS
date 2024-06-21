// Includes
#include "system.h"
#include "font.h"
#include "screen.h"

// Set the initial seed
uint32_t rseed = 1;

// Sets the seed
void seed(uint32_t s) { rseed = s; }

// Gets a pseudorandom number
uint32_t rand() {
    rseed = rseed * 1103515245 + 12345;
    return (unsigned int)(rseed / 65536) % 32768;
}

// Halts the program and displays an error message
void panic(const char *err) {
    screen_clear(COLOUR(7, 0, 0));

    if (err != NULL) {
        font_str(err, (SCREEN_WIDTH - font_width(err)) / 2,
                 SCREEN_HEIGHT / 2 - 4, COLOUR(7, 7, 3));
    } else {
        char str[] = "An exception has occured";
        font_str(str, (SCREEN_WIDTH - font_width(str)) / 2,
                 SCREEN_HEIGHT / 2 - 4, COLOUR(7, 7, 3));
    }

    screen_update();
    for (;;) {}
}
