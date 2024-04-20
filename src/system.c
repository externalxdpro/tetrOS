#include "system.h"
#include "tty.h"

void panic(const char *err) {
    tty_clearscreen();

    if (err != NULL) {
        tty_writestring(err);
    } else {
        tty_writestring("An exception has occured");
    }

    for (;;) {}
}
