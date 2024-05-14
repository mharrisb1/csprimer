/*
        @author Michael Harris
        @date   2024-05-13

        FSM that processes a single token from a line in the CSS file and compiles
        hexidecimal form to RGB form.

        **Only works for hex form of exactly 3 bytes**

        State machine:
                - Input alphabet (Sigma) is any valid UTF-8 character from CSS file(s)
                - States (S) are:
                        - ACCEPT (0): initial state allowing zero transistions to be acceptable
                        - CONSUME (1): will store received token in buffer. If `;` is found will transistion and not store
                        - CONVERT (2): converts all values in buffer to RGB form until exhausted
                        - ERROR (3): a non-hex value or `;` is found during consumption state
                - Initial state (s_0) is ACCEPT

        Funcion signature:

                void process_token(const char t, State *state, char *buf, char *ix);

        Notes:
                - Still a little fuzzy on frame pointer and return address (link register)
                - Remove use of `bhi` instruction since I don't need to handle case when state value not in range [0, 3]
*/

        .text
        .global process_token
        .type process_token, @function

/* Constants */

.C0:
        .string "rgb("

process_token:
        stp x29, x30, [sp, -64]!        // store current values of the frame pointer and link register to the stack
        mov x29, sp                     // set frame pointer to current value of stack pointer
        strb w0, [sp, 47]               // store value of token on stack
        str x1, [sp, 32]                // store reference to state on stack
        str x2, [sp, 24]                // store reference to buffer on stack
        str x3, [sp, 16]                // store reference to buffer index on stack
        ldr x0, [sp, 32]                // load reference to state from stack
        ldr w0, [x0]                    // load value of state
        cmp w0, 3                       // *state == ERROR
        beq .ERROR
        cmp w0, 2                       // *state == CONVERT
        beq .CONVERT
        cmp w0, 1                       // *state == CONSUME
        beq .CONSUME
        b .ACCEPT                       // only remaining option is ACCEPT state so can just jump to that

.ACCEPT:
        ldrb w0, [sp, 47]               // load token value from stack
        cmp w0, 0x23                    // is the token the start of a hex sequence? t == '#'
        bne .print_tok
        ldr x0, [sp, 32]                // load reference to state from stack
        mov w1, 1
        str w1, [x0]                    // *state = CONSUME
        ldr x0, [sp, 16]                // load reference to buffer index from stack
        strb wzr, [x0]                  // reset buffer index to 0
        b .end

.CONSUME:
        ldrb w0, [sp, 47]               // load token value from stack
        cmp w0, 0x3B                    // is the token the terminator? t == ';'
        bne .is_hex
        ldr x0, [sp, 32]                // load reference to state from stack
        mov w1, 2
        str w1, [x0]                    // *state = CONVERT
        b .end

.print_tok:
        ldrb w0, [sp, 47]               // load token value from stack
        bl putchar                      // branch to address of stdlib `putchar` func to print token
        b .end

.is_hex:
        ldrb w0, [sp, 47]               // load token value from stack
        bl isxdigit                     // branch to address of stdlib `isxdigit` func
        cbnz w0, .enqueue               // if result of `isxdigit` is not zero then move on to add token to buffer
        ldr x0, [sp, 32]                // load reference to state from stack
        mov w1, 3
        str w1, [x0]                    // *state = ERROR
        b .end
        
.enqueue:
        /* FIXME: post-increment ix */
        ldr x0, [sp, 16]                // load reference to buffer index from stack
        ldrb w1, [x0]                   // dereference pointer
        add w1, w1, 1                   // increment value
        strb w1, [x0]                   // store incremented value at reference location
        ldr x0, [sp, 24]                // load reference to buffer from stack
        add x0, x1, x0                  // increment address by buffer index as offset
        ldrb w1, [sp, 47]               // load token value from stack
        strb w1, [x0]                   // store token at buf[ix]
        b .end

.end:
        ldp x29, x30, [sp], 64          // restore original values of frame pointer and link register and deallocate from stack
        ret
 
