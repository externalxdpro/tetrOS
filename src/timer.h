#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define TIMER_TPS 100

uint64_t timer_get();
void     timer_wait(uint64_t ticks);
void     timer_install();

#endif // TIMER_H_
