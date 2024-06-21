#ifndef SYSTEM_H_
#define SYSTEM_H_

// Includes
#include <stdint.h>

// Functions
void     seed(uint32_t s);
uint32_t rand();

void panic(const char *err);

#endif // SYSTEM_H_
