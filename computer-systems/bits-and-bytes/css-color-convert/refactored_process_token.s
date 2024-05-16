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

.equ INITIATOR_TOK,    0x23 // #
.equ TERMINATOR_TOK,   0x3B // ;
.equ ACCEPT,           0x00
.equ CONSUME,          0x01
.equ CONVERT,          0x02
.equ ERROR,            0x03

        .text
        .global process_token
        .type process_token, @function

process_token:
        /* allocate bytes to stack and save frame pointer and link register */
        stp x29, x30, [sp, -80]!
        mov x29, sp

        /* save callee-saved registers */
        str x19, [sp, 16]
        str x20, [sp, 24]
        str x21, [sp, 32]
        str x22, [sp, 40]
        str x23, [sp, 48]
        str x24, [sp, 56]
        str x25, [sp, 64]
        str x26, [sp, 72]
        
        /* core values for operations */
        mov w19, w0                // token value
        mov x20, x1                // state reference
        ldrb w21, [x20]            // dereferenced state value
        mov x22, x3                // buffer index reference
        ldrb w23, [x22]            // dereferenced buffer index
        mov x24, x2                // buffer reference
        mov w25, 2                 // alt buffer index
        mov x26, xzr               // temp token store

        /* NOTE: will never enter here in ERROR state */
.switch:
        cmp w21, CONVERT
        beq .converting
        cmp w21, CONSUME
        beq .consuming

.accepting:
        mov w1, CONSUME            // eagerly set next state
        cmp w19, INITIATOR_TOK
        beq .transition
        bl putchar                 // token is already in w0 register
        b .end

.consuming:
        mov w1, CONVERT
        cmp w19, TERMINATOR_TOK
        beq .transition
        mov w1, ERROR
        bl isxdigit                // token is already in w0 register
        cbnz w0, .enqueue
        b .transition

.converting:
        ldr x0, =opening_seq
        bl printf

.convert_pair:
        ldrb w26, [x24, x25]
        strb wzr, [x24, x25]
        sub w25, w25, 2
        add x0, x24, x25
        mov w1, 0
        mov w2, 16
        bl strtol
        mov w1, w0
        ldr x0, =uintfs
        bl printf
        add w25, w25, 2
        strb w26, [x24, x25]
        add w25, w25, 2
        cmp w25, 8
        beq .done_converting
        mov w0, 0x20
        bl putchar
        b .convert_pair

.done_converting:
        ldr x0, =closing_seq
        bl printf
        mov w1, ACCEPT
        b .transition

.enqueue:
        add w1, w23, 1
        strb w1, [x22]             // save incremented value
        strb w19, [x24, x23]       // write token to buffer offset loc
        b .end

.transition:
        strb w1, [x20]             // write to state
        strb wzr, [x22]            // reset buffer index to 0

.end:
        /* restore callee-saved registers */
        ldr x19, [sp, 16]
        ldr x20, [sp, 24]
        ldr x21, [sp, 32]
        ldr x22, [sp, 40]
        ldr x23, [sp, 48]
        ldr x24, [sp, 56]
        ldr x25, [sp, 64]
        ldr x26, [sp, 72]

        /* deallocate bytes from stack and restore frame pointer and link register */
        ldp x29, x30, [sp], 80
        ret

        .data

opening_seq:        .string "rgb("
closing_seq:        .string ");\n"
uintfs:             .string "%u"
