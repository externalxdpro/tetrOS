#include "system.h"
#include "screen.h"

void panic(const char *err) {
    screen_clear(COLOUR(7, 0, 0));

    /* if (err != NULL) { */
    /*     tty_writestring(err); */
    /* } else { */
    /*     tty_writestring("An exception has occured"); */
    /* } */

    for (;;) {}
}
