/*
 *        Validates a 16 character sequence of digits using Luhn algorithm
 *
 *        int is_valid(char digits[16]);
 *
 *        @param digits - 16-byte digits sequence
 *
 *        Register glossary
 *        -----------------
 *        X1 - index (i)
 *        X2 - accumulator (k)
 *        X3 - factor (f)
 *        X4 - digits number (n)
 *        X5 - lookup address (*l)
 */

.section .data,"aw"
.align 2
lookup:
    .byte 0x00
    .byte 0x02
    .byte 0x04
    .byte 0x06
    .byte 0x08
    .byte 0x01
    .byte 0x03
    .byte 0x05
    .byte 0x07
    .byte 0x09
    .byte 0x00
    .byte 0x01
    .byte 0x02
    .byte 0x03
    .byte 0x04
    .byte 0x05
    .byte 0x06
    .byte 0x07
    .byte 0x08
    .byte 0x09

.section .text,"ax"
.global _is_valid

_is_valid:
    mov w1, wzr                // i
    mov w2, wzr                // k
    mov w3, wzr                // f
    adrp x5, lookup            // Load the page address of lookup into x5
    add x5, x5, :lo12:lookup   // Add the page offset of lookup to x5
    mov w6, 0x0A               // static mask
    mov w7, 0x30               // static mask

.loop:
    cmp w1, 0x10               // i == 16
    b.eq .mod10
    ldrb w4, [x0, w1, uxtw]    // n = digits[i]
    eor w4, w4, w7             // n = uint(n)
    add w4, w4, w3             // n += f
    ldrb w4, [x5, w4, uxtw]    // n = lookup[n]
    add w2, w2, w4             // k += n
    eor w3, w3, w6             // f ^= 0x0A
    add w1, w1, 1              // i++
    b .loop

.mod10:
    udiv w0, w2, w6
    mul w0, w0, w6
    sub w0, w2, w0

.end:
    eor w0, w0, w6
    ret
