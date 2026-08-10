.syntax unified
.cpu cortex-m4
.thumb

.global asm_critical_section
.type asm_critical_section, %function

asm_critical_section:
    PUSH    {r4-r7, lr}

    LDR     r4, [r0]          /* Load volatile status */
    TST     r4, #0x01
    BEQ     .Ldone

    MOVS    r5, r1            /* Save length */
    MOVS    r6, r2            /* Save buffer */

.Lloop:
    CMP     r5, #0
    BEQ     .Ldone

    LDRB    r7, [r6]
    STRB    r7, [r0, #4]      /* Write to data register */

    SUBS    r5, r5, #1
    B       .Lloop

.Ldone:
    POP     {r4-r7, pc}