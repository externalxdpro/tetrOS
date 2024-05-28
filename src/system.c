#include "system.h"
#include "font.h"
#include "screen.h"

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
