#ifndef TIMER_H_
#define TIMER_H_

// Includes
#include <stdint.h>

// Defines
#define TIMER_TPS 100

// Functions
uint64_t timer_get();
void     timer_wait(uint64_t ticks);
void     timer_install();

#endif // TIMER_H_
