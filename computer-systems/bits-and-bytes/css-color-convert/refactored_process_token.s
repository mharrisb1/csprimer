/*
        @author Michael Harris
        @date   2024-05

        FSM that processes a single token from a line in the CSS file and compiles hexidecimal form to RGB form.

        **Only works for hex form of exactly 3 bytes**

        State machine:
                - Input alphabet (Sigma) is any valid UTF-8 character from CSS file(s)
                - States (S) are:
                        - ACCEPT (0): initial state allowing zero transistions to be acceptable
                        - CONSUME (1): will store received token in buffer. Terminates at `;`
                        - CONVERT (2): converts all values in buffer to RGB form until exhausted
                        - ERROR (3): a non-hex value or `;` is found during consumption state
                - Initial state (s_0) is ACCEPT
                - Accepting states = {ACCEPT}

        Function signature:

                void process_token(const char t, State *state, char *buf, char *ix);

*/

        .text
        .global process_token
        .type process_token, @function

process_token:
        /* allocate bytes to stack and save frame pointer and link register */
        stp x29, x30, [sp, -64]!
        mov x29, sp

        /* save callee-saved registers */
        str x19, [sp, 16]
        str x20, [sp, 24]
        str x21, [sp, 32]
        str x22, [sp, 40]
        str x23, [sp, 48]
        str x24, [sp, 56]
        
        /* core values for operations */
        mov w19, w0                // token value
        mov x20, x1                // state reference
        mov w21, [x20]             // dereferenced state value
        mov x22, x3                // buffer index reference
        mov w23, [x22]             // dereferenced buffer index
        mov x24, x2                // buffer reference

        /* NOTE: will never enter here in ERROR state */
.switch:
        cmp w21, 2
        beq .converting
        cmp w21, 1
        beq .consuming

.accepting:
        cmp w19, =initiator
        beq .transconsume
        bl putchar                 // token is already in w0 register
        b .end

.consuming:
        nop

.transconsume:
        nop

.end:
        /* restore callee-saved registers */
        ldr x19, [sp, 16]
        ldr x20, [sp, 24]
        ldr x21, [sp, 32]
        ldr x22, [sp, 40]
        ldr x23, [sp, 48]
        ldr x24, [sp, 56]

        /* deallocate bytes from stack and restore frame pointer and link register */
        ldp x29, x30, [sp], 64
        ret

        .data
initiator:        .byte 0x23       // #
terminator:       .byte 0x3B       // ;
