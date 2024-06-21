// Includes
#include "timer.h"
#include "irq.h"
#include "isr.h"
#include "util.h"

// The current number of ticks
volatile uint64_t timer_ticks = 0;

// Sets the Hz of the timer
void timer_phase(int hz) {
    uint16_t divisor = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, divisor >> 8);
}

// Returns the current tick count
uint64_t timer_get() { return timer_ticks; }

// Halt the program for a specified number of ticks
void timer_wait(uint64_t ticks) {
    uint64_t eticks = timer_ticks + ticks;
    while (timer_ticks < eticks) {
        asm("sti//hlt//cli");
    }
}

// Increments the tick counter
void timer_handler(struct registers *r) { timer_ticks++; }

// Sets up timer into IRQ
void timer_install() {
    timer_phase(TIMER_TPS);
    irq_install_handler(0, timer_handler);
}
