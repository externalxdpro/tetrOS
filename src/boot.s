.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set VIDMODE,   0x00000004
.set FLAGS,     ALIGN | MEMINFO | VIDMODE
.set MAGIC,     0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)


.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.long   0
.long   0
.long   0
.long   0
.long   0

;; Set resolution
.long 0
.long 320
.long 200
.long 8

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:


;; Runs the main function
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp


    call kernel_main


    cli
1:  hlt
    jmp 1b


.size _start, . - _start


;; GDT stuff
.global gdt_flush
.extern gp
gdt_flush:
    lgdt [gp]
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $flush2

flush2:
    ret


;; IDT stuff
.global idt_load
.extern idtp
idt_load:
    lidt [idtp]
    ret


;; Macros for if the ISR register has an error associated or not
.macro ISR_NO_ERR index
    .global _isr\index
    _isr\index:
        cli
        push $0
        push $\index
        jmp isr_common_stub
.endm

.macro ISR_ERR index
    .global _isr\index
    _isr\index:
        cli
        push $\index
        jmp isr_common_stub
.endm

// Set ISRs
ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31

;; Handles stuff for ISR
isr_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    mov $fault_handler, %eax
    call %eax
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

;; Sets IRQs
.global _irq0
.global _irq1
.global _irq2
.global _irq3
.global _irq4
.global _irq5
.global _irq6
.global _irq7
.global _irq8
.global _irq9
.global _irq10
.global _irq11
.global _irq12
.global _irq13
.global _irq14
.global _irq15

_irq0:
    cli
    push $0
    push $32
    jmp irq_common_stub

_irq1:
    cli
    push $0
    push $33
    jmp irq_common_stub

_irq2:
    cli
    push $0
    push $34
    jmp irq_common_stub

_irq3:
    cli
    push $0
    push $35
    jmp irq_common_stub

_irq4:
    cli
    push $0
    push $36
    jmp irq_common_stub

_irq5:
    cli
    push $0
    push $37
    jmp irq_common_stub

_irq6:
    cli
    push $0
    push $38
    jmp irq_common_stub

_irq7:
    cli
    push $0
    push $39
    jmp irq_common_stub

_irq8:
    cli
    push $0
    push $40
    jmp irq_common_stub

_irq9:
    cli
    push $0
    push $41
    jmp irq_common_stub

_irq10:
    cli
    push $0
    push $42
    jmp irq_common_stub

_irq11:
    cli
    push $0
    push $43
    jmp irq_common_stub

_irq12:
    cli
    push $0
    push $44
    jmp irq_common_stub

_irq13:
    cli
    push $0
    push $45
    jmp irq_common_stub

_irq14:
    cli
    push $0
    push $46
    jmp irq_common_stub

_irq15:
    cli
    push $0
    push $47
    jmp irq_common_stub

;; Handles IRQ stuff
.extern irq_handler
irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    mov $irq_handler, %eax
    call %eax
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret
