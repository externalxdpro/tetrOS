// Sakib Ahmed
// TetrOS
// An operating system (more like a kernel) that just plays Tetris

// Includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "game.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include "util.h"

// Check if the OS is being run from the correct architecture
#if defined(__linux__)
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

// Main function
void kernel_main() {
    // Run the setup functions
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    screen_install();
    timer_install();
    keyboard_install();
    generate_sprites();

    asm("sti");

    // Enable the menu
    state.menu = true;

    // Keep track of the last frame
    uint32_t last_frame = 0;

    // Main loop
    while (true) {
        // Get the current tick count
        const uint32_t now = (uint32_t)timer_get();

        // Check if it has been a frame
        if ((now - last_frame) > (TIMER_TPS / FPS)) {
            last_frame = now;

            // Render the menu if the menu is enabled
            // Else render the game
            if (state.menu) {
                update_menu();
                render_menu();
            } else {
                update();
                render();
            }

            // Update the screen and add to the frame counter
            screen_update();
            state.frames++;
        }
    }
}
