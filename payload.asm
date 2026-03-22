.section .text
.global _start

_start:
    // 1. Heartbeat to Teensy (ASCII '#')
    mov  w0, #0x23
    ldr  x1, =0xE0001000    // Southbridge UART TX register
    strb w0, [x1]

    // 2. Patch hypervisor signature check (PLACEHOLDER ADDRESS)
    //    Replace 0xFFFF0000 + OFFSET with actual address after analysis.
    ldr  x2, =0xFFFF000040   // <- CHANGE THIS after finding real offset
    mov  w3, #0x0            // return 0 (success)
    str  w3, [x2]            // Overwrite branch with NOP or success

    // 3. Dump bootrom (first 256 bytes)
    ldr  x4, =0xFFFF0000
    mov  w5, #256
    bl   dump_hex

    // 4. Hang forever (or return – but returning may continue boot)
    b    .

// ------------------------------------------------------------
// Subroutine: dump_hex (x4 = address, w5 = count)
dump_hex:
    stp  x29, x30, [sp, #-32]!
    mov  x29, sp
    str  x4, [sp, #16]
    str  w5, [sp, #24]
    mov  w8, #0               // column counter
    ldr  x6, =0xE0001000      // UART TX

1:  subs w5, w5, #1
    b.lt 4f
    ldrb w7, [x4], #1

    // High nibble
    lsr  w9, w7, #4
    cmp  w9, #10
    add  w9, w9, #'0'
    blt  2f
    add  w9, w9, #7
2:  strb w9, [x6]

    // Low nibble
    and  w9, w7, #0xF
    cmp  w9, #10
    add  w9, w9, #'0'
    blt  3f
    add  w9, w9, #7
3:  strb w9, [x6]

    // Space every 16 bytes
    add  w8, w8, #1
    and  w9, w8, #0xF
    cbnz w9, 1b
    mov  w9, #' '
    strb w9, [x6]
    b    1b

4:  // newline
    mov  w9, #'\r'
    strb w9, [x6]
    mov  w9, #'\n'
    strb w9, [x6]

    ldp  x29, x30, [sp], #32
    ret
