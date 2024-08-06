/*
 *        @author Michael Harris
 *        @date 2024-07-31
 *
 *        Validates a 16 character sequence of digits using Luhn algorithm
 *
 *        int is_valid(char digits[16]);
 *
 *        @param digits - Digits sequence
*/

.data
        table: .word 0, 2, 4, 6, 8, 1, 3, 5, 7, 9

.text
        .global _is_valid

_is_valid:
        mov w2, wzr                // n
        mov w3, wzr                // i
        mov w4, wzr                // c

.loop:
        ldr w5, [x1, x3]
        eor w5, w5, 0x30
        add w3, w3, 1
        cmp w3, 15
        b.eq .end

.o1:
        

.o2:

.end:
        

_is_valid:
        sub sp, sp, 16               // allocate space on stack

        /* @dev first 20 bytes are reserved for copy buffer */
        str wzr, [sp, 20]            // N (accumulator)
        str wzr, [sp, 24]            // K (len of digits)
        str wzr, [sp, 28]            // C (check digit)

/**/
loop1:
        ldr w1, [sp, 24]             // K
        ldr w2, [x0, x1]             // D[K]
        str w2, [sp, x1]             // SP[K] = D[K]
        str w2, [sp, 28]             // C = D[K]
        cmp w2, wzr                  // D[K] == 0
        b.eq loop2
        add w1, w1, 1                // K++
        str w1, [sp, 24]             // K
        mov w3, w1                   // copy K
        b loop1

loop2:
        sub w1, w1, 1                // K--
        cmp w1, wzr                  // K == 0
        b.eq end
        sub w2, w3, w1               // copy K - K

end:
        add sp, sp, 32               // deallocate space on stack
        ret
