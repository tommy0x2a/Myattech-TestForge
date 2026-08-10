/**
 * startup.s
 * Realistic ARM Cortex-M4 startup / vector table + example functions
 * for TestForge M4 assembly parser validation.
 */

.syntax unified
.cpu cortex-m4
.thumb

.section .text, "ax", %progbits
.global Reset_Handler
.global SysTick_Handler
.global memcpy_fast
.global delay_cycles
.type Reset_Handler, %function
.type SysTick_Handler, %function
.type memcpy_fast, %function
.type delay_cycles, %function

/* Weak alias example */
.weak Reset_Handler

Reset_Handler:
    /* Standard Cortex-M reset handler prologue */
    PUSH {r4-r7, lr}

    /* Initialize system */
    BL SystemInit

    /* Call into C main */
    BL main

    /* Should never reach here */
    POP {r4-r7, pc}

SysTick_Handler:
    PUSH {r0-r3, lr}

    /* Increment tick counter (example load/store) */
    LDR r0, =tick_counter
    LDR r1, [r0]
    ADDS r1, r1, #1
    STR r1, [r0]

    POP {r0-r3, pc}

memcpy_fast:
    PUSH {r4-r8, lr}

    /* Optimized copy using LDMIA/STMIA */
    MOVS r4, r0
    MOVS r5, r1
    MOVS r6, r2

memcpy_loop:
    CMP r6, #16
    BLT memcpy_tail

    LDMIA r5!, {r0-r3}
    STMIA r4!, {r0-r3}
    SUBS r6, r6, #16
    B memcpy_loop

memcpy_tail:
    CMP r6, #0
    BEQ memcpy_done

    LDRB r0, [r5]
    STRB r0, [r4]
    ADDS r5, r5, #1
    ADDS r4, r4, #1
    SUBS r6, r6, #1
    B memcpy_tail

memcpy_done:
    POP {r4-r8, pc}

delay_cycles:
    /* Leaf function: argument in r0, no stack frame */
    MOVS r1, r0

delay_loop:
    SUBS r1, r1, #1
    BNE delay_loop

    BX lr

/* Example data section to test data label skipping heuristic */
.section .data
my_table:
    .word 0x00000001
    .word 0x00000002

.section .bss
tick_counter:
    .word 0
