.equ INTC_BASE, 0x48200000

/* CPSR */
.equ CPSR_I,   0x80
.equ CPSR_F,   0x40
.equ CPSR_IRQ, 0x12
.equ CPSR_USR, 0x10
.equ CPSR_FIQ, 0x11
.equ CPSR_SVC, 0x13
.equ CPSR_ABT, 0x17
.equ CPSR_UND, 0x1B
.equ CPSR_SYS, 0x1F

/* Vector table */
_vector_table:
    ldr pc, _reset
    ldr pc, _undf
    ldr pc, _swi
    ldr pc, _pabt
    ldr pc, _dabt
    nop
    ldr pc, _irq
    ldr pc, _fiq

_reset: .word _start
_undf:  .word 0x4030CE24
_swi:   .word 0x4030CE28
_pabt:  .word 0x4030CE2C
_dabt:  .word 0x4030CE30
        nop
_irq:   .word 0x4030CE38
_fiq:   .word 0x4030CE3C

_start:
    /* Set V=0 in CP15 SCTRL register - for VBAR to point to vector */
    mrc p15, 0, r0, c1, c0, 0
    bic r0, #(1 << 13)
    mcr p15, 0, r0, c1, c0, 0

    /* Set vector address in CP15 VBAR register */
    ldr r0, =_vector_table
    mcr p15, 0, r0, c12, c0, 0

    /* Inicializa SP modo IRQ */
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #CPSR_IRQ
    msr cpsr, r0
    ldr sp, =0x4030B000

    /* Inicializa SP modo SVC, FIQ off, IRQ on */
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #CPSR_SVC
    orr r0, r0, #(CPSR_F)
    and r0, r0, #~(CPSR_I)
    msr cpsr, r0
    ldr sp, =0x4030A000

    /* Escreve endereco do handler na tabela de vetor RAM */
    ldr r0, =_irq
    ldr r1, =.irq_handler
    str r1, [r0]

    bl _main

.loop: b .loop

.irq_handler:
    stmfd sp!, {r0-r12, lr}
    mrs r11, spsr
    bl ISR_Handler
    dsb
    msr spsr, r11
    ldmfd sp!, {r0-r12, lr}
    subs pc, lr, #4