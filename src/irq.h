#ifndef IRQ_H_
#define IRQ_H_

#include "isr.h"

void irq_install_handler(int irq, void (*handler)(struct registers *r));
void irq_uninstall_handler(int irq);
void irq_install();

#endif // IRQ_H_
